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
#ifndef __LOGWINDOW_H__
#define __LOGWINDOW_H__

#include <exception>
#include <iostream>

#include <OgreLog.h>
#include "ImguiOgre/ImguiManager.h"

using namespace Ogre;



struct element  {
    String msg;
    LogMessageLevel lml;
};

class LogWindow : public LogListener
{
public:
    LogWindow();
    virtual ~LogWindow();

    // void draw();
    void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName, bool& skipThisMessage ) override;
    void Clear();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);
    void DrawWindow(const char* title, bool* p_open = NULL);

    // bool show = false;
    ImFont* font;
protected:
    std::list<element> items;

    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.
    String              LogFile;

};


#endif
