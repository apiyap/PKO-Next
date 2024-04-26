/*-------------------------------------------------------------------------
This source file is a part of ...

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/
#include <exception>
#include <iostream>

#include "LogWindow.h"



LogWindow::LogWindow()
:LogListener()
{
    //show = true;
    font = NULL;
    items = {};

    AutoScroll = true;
    Clear();
}

LogWindow::~LogWindow()
{
    
}

void LogWindow::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}

void LogWindow::AddLog(const char* fmt, ...)
{
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size + 1);
}



void LogWindow::messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName, bool& skipThisMessage )
{

    // logName = location 
    LogFile = logName;
    String level = "";
    switch (lml)
    {
        case LML_TRIVIAL:
            level = "TRIVIAL";
        break;
        case LML_NORMAL:
            level = "NORMAL";
        break;
        // case LML_WARNING:
        //     level = "WARNING";
        // break;
        case LML_CRITICAL:
            level = "CRITICAL";
        break;
        default:
        break;
    }
    AddLog((level +" : "+ message+"\n" ).c_str());

}

// void LogWindow::draw()
// {
//     if(!show) return;

//     ImGui::SetNextWindowSize({500, 400}, ImGuiCond_FirstUseEver);

//     show = ImGui::Begin("Log", &show);
//     ImGui::PushFont(font);
//     for(auto& it : items)
//     {
//         auto msg = it.msg;
//         auto lvl = it.lml;
//         if(lvl==LML_CRITICAL)
//         {
//             ImGui::PushStyleColor(ImGuiCol_Text, {1, 0.4, 0.4, 1});
//         }else if(lvl ==LML_WARNING)
//         {
//             ImGui::PushStyleColor(ImGuiCol_Text, {1, 0.8, 0.4, 1});
//         }

//         ImGui::TextWrapped(msg.c_str());

//         if(lvl > LML_NORMAL)
//         {
//             ImGui::PopStyleColor();
//         }

//     }

//     // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
//     // Using a scrollbar or mouse-wheel will take away from the bottom edge.
//     if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
//             ImGui::SetScrollHereY(1.0f);

//     ImGui::PopFont();

//     ImGui::End();

// }



void LogWindow::DrawWindow(const char* title, bool* p_open)
{
    if (!ImGui::Begin(title, p_open))
    {
        ImGui::End();
        return;
    }    
    ImGui::PushFont(font);
    ImGui::TextUnformatted(LogFile.c_str());
    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have random access to the result of our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of
            // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
            // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
            // within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
            // on your side is recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we have an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display
            // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
            // it possible (and would be recommended if you want to search through tens of thousands of entries).
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::PopFont();
    ImGui::End();


}