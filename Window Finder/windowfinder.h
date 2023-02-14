// Window Finder
// https://github.com/Apex-master

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>

std::string tstring_to_string(const TCHAR* tstr)
{
#ifdef UNICODE
    std::wstring wstr(tstr);
    return std::string(wstr.begin(), wstr.end());
#else
    return std::string(tstr);
#endif
}

bool hide_child = false;

std::map<DWORD, std::string> windowStyles =
{
    {WS_OVERLAPPED, "WS_OVERLAPPED"},
    {WS_POPUP, "WS_POPUP"},
    {WS_CHILD, "WS_CHILD"},
    {WS_MINIMIZE, "WS_MINIMIZE"},
    {WS_VISIBLE, "WS_VISIBLE"},
    {WS_DISABLED, "WS_DISABLED"},
    {WS_CLIPSIBLINGS, "WS_CLIPSIBLINGS"},
    {WS_CLIPCHILDREN, "WS_CLIPCHILDREN"},
    {WS_MAXIMIZE, "WS_MAXIMIZE"},
    {WS_CAPTION, "WS_CAPTION"},
    {WS_BORDER, "WS_BORDER"},
    {WS_DLGFRAME, "WS_DLGFRAME"},
    {WS_VSCROLL, "WS_VSCROLL"},
    {WS_HSCROLL, "WS_HSCROLL"},
    {WS_SYSMENU, "WS_SYSMENU"},
    {WS_THICKFRAME, "WS_THICKFRAME"},
    {WS_GROUP, "WS_GROUP"},
    {WS_TABSTOP, "WS_TABSTOP"},
    {WS_MINIMIZEBOX, "WS_MINIMIZEBOX"},
    {WS_MAXIMIZEBOX, "WS_MAXIMIZEBOX"},
    {WS_TILED, "WS_TILED"},
    {WS_ICONIC, "WS_ICONIC"},
    {WS_SIZEBOX, "WS_SIZEBOX"},
    {WS_POPUPWINDOW, "WS_POPUPWINDOW"},
    {WS_OVERLAPPEDWINDOW, "WS_OVERLAPPEDWINDOW"},
    {WS_TILEDWINDOW, "WS_TILEDWINDOW"},
    {WS_CHILDWINDOW, "WS_CHILDWINDOW"},
};

std::map<DWORD, std::string> extendedWindowStyles =
{
    {WS_EX_DLGMODALFRAME, "WS_EX_DLGMODALFRAME"},
    {WS_EX_NOPARENTNOTIFY, "WS_EX_NOPARENTNOTIFY"},
    {WS_EX_TOPMOST, "WS_EX_TOPMOST"},
    {WS_EX_ACCEPTFILES, "WS_EX_ACCEPTFILES"},
    {WS_EX_TRANSPARENT, "WS_EX_TRANSPARENT"},
    {WS_EX_MDICHILD, "WS_EX_MDICHILD"},
    {WS_EX_TOOLWINDOW, "WS_EX_TOOLWINDOW"},
    {WS_EX_WINDOWEDGE, "WS_EX_WINDOWEDGE"},
    {WS_EX_CLIENTEDGE, "WS_EX_CLIENTEDGE"},
    {WS_EX_CONTEXTHELP, "WS_EX_CONTEXTHELP"},
    {WS_EX_RIGHT, "WS_EX_RIGHT"},
    {WS_EX_LEFT, "WS_EX_LEFT"},
    {WS_EX_RTLREADING, "WS_EX_RTLREADING"},
    {WS_EX_LTRREADING, "WS_EX_LTRREADING"},
    {WS_EX_LEFTSCROLLBAR, "WS_EX_LEFTSCROLLBAR"},
    {WS_EX_RIGHTSCROLLBAR, "WS_EX_RIGHTSCROLLBAR"},
    {WS_EX_CONTROLPARENT, "WS_EX_CONTROLPARENT"},
    {WS_EX_STATICEDGE, "WS_EX_STATICEDGE"},
    {WS_EX_APPWINDOW, "WS_EX_APPWINDOW"},
    {WS_EX_OVERLAPPEDWINDOW, "WS_EX_OVERLAPPEDWINDOW"},
    {WS_EX_PALETTEWINDOW, "WS_EX_PALETTEWINDOW"},
    {WS_EX_LAYERED, "WS_EX_LAYERED"},
    {WS_EX_NOINHERITLAYOUT, "WS_EX_NOINHERITLAYOUT"},
    {WS_EX_NOREDIRECTIONBITMAP, "WS_EX_NOREDIRECTIONBITMAP"},
    {WS_EX_LAYOUTRTL, "WS_EX_LAYOUTRTL"},
    {WS_EX_COMPOSITED, "WS_EX_COMPOSITED"},
    {WS_EX_NOACTIVATE, "WS_EX_NOACTIVATE"}
};

struct AppWindow
{
    std::string window_name;
    std::string window_class;
    std::string process_name;

    HWND hwnd;

    int process_id;
    int thread_id;

    RECT rect;
    
    std::vector<std::string> window_styles;
    std::vector<std::string> window_extendedstyles;
};

std::vector<AppWindow> windows;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    DWORD process_id, thread_id;
    thread_id = GetWindowThreadProcessId(hwnd, &process_id);

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);
    if (process)
    {
        TCHAR process_name[MAX_PATH];
        if (GetModuleFileNameEx(process, NULL, process_name, MAX_PATH))
        {
            // hwnd
            AppWindow app;
            app.hwnd = hwnd;

            // process name
            app.process_name = tstring_to_string(process_name);

            //Identify if it is a child window
            HWND parent = GetParent(hwnd);
            if (parent != NULL && !hide_child)
            {
                DWORD ownerID;
                GetWindowThreadProcessId(parent, &ownerID);

                // Window name
                int lenO = GetWindowTextLength(hwnd);
                TCHAR* window_name = new TCHAR[lenO + 1];
                GetWindowText(hwnd, window_name, lenO + 1);

                app.window_name = "[Child Window] [Owner Proc ID:" + std::to_string(ownerID) + "] " + tstring_to_string(window_name);
            }
            else
            {
                // Window name
                int len = GetWindowTextLength(hwnd);
                TCHAR* window_name = new TCHAR[len + 1];
                GetWindowText(hwnd, window_name, len + 1);
                app.window_name = tstring_to_string(window_name);
            }

            // Window class
            TCHAR windowClass[256];
            GetClassName(hwnd, windowClass, 256);
            app.window_class = tstring_to_string(windowClass);

            //Process ID
            app.process_id = process_id;

            // Thread ID
            app.thread_id = thread_id;

            // Window size
            GetWindowRect(hwnd, &app.rect);


            DWORD flags = GetWindowLong(hwnd, GWL_STYLE);

            //Window styles
            for (const auto& style : windowStyles)
            {
                if (flags & style.first)
                {
                    app.window_styles.push_back(style.second);
                }

            }

            //Window extended styles
            for (const auto& style : extendedWindowStyles)
            {
                if (flags & style.first)
                {
                    app.window_extendedstyles.push_back(style.second);
                }

            }

            windows.push_back(app);

            if (!hide_child)
            {
                EnumChildWindows(hwnd, EnumWindowsProc, NULL);
            }
        }
        CloseHandle(process);
    }

    return TRUE;
}

void PopupText(const AppWindow& app)
{
    ImGui::Text("Window name: %s\nClass name: %s\nProcess name: %s\nHWND: %d\nProcess ID: %d\nThread ID: %d\nRect: (%d x %d)", app.window_name.c_str(), app.window_class.c_str(), app.process_name.c_str(), app.hwnd, app.process_id, app.thread_id, app.rect.left, app.rect.right - app.rect.left, app.rect.bottom - app.rect.top);
}

std::string searchTerm;
int selected_row = -1;

void PrintWindows(const AppWindow& app, int i)
{
    ImGui::TableNextRow();

    // ID
    ImGui::TableSetColumnIndex(0);
    char buffer4[32];
    snprintf(buffer4, sizeof(buffer4), "%i", i);
    ImGui::Selectable(buffer4);

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Window Name
    ImGui::TableSetColumnIndex(1);
    ImGui::Selectable(app.window_name.c_str());
    if (ImGui::IsItemClicked())
    {
        selected_row = i;

    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Window Class
    ImGui::TableSetColumnIndex(2);
    ImGui::Selectable(app.window_class.c_str());

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Process Name
    ImGui::TableSetColumnIndex(3);
    ImGui::Selectable(app.process_name.c_str());

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Process ID
    ImGui::TableSetColumnIndex(4);
    char buffer3[32];
    snprintf(buffer3, sizeof(buffer3), "%i", app.process_id);
    ImGui::Selectable(buffer3);

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Thread ID
    ImGui::TableSetColumnIndex(5);
    char buffer2[32];
    snprintf(buffer2, sizeof(buffer2), "%i", app.thread_id);
    ImGui::Selectable(buffer2);

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // HWND
    ImGui::TableSetColumnIndex(6);
    char buffer1[32];
    snprintf(buffer1, sizeof(buffer1), "0x%X", (unsigned int)app.hwnd);
    ImGui::Selectable(buffer1);

    if (ImGui::IsItemClicked())
    {
        selected_row = i;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Window Size
    ImGui::TableSetColumnIndex(7);
    char buffer[100];
    snprintf(buffer, 100, "%d x %d", app.rect.right - app.rect.left, app.rect.bottom - app.rect.top);
    ImGui::Selectable(buffer);

    if (ImGui::IsItemClicked())
    {
        selected_row = i;

    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        PopupText(app);
        ImGui::EndTooltip();
    }

    // Window Styles
    ImGui::TableSetColumnIndex(8);
    for (const auto& style : app.window_styles)
    {
        ImGui::Selectable(style.c_str());

        if (ImGui::IsItemClicked())
        {
            selected_row = i;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            PopupText(app);
            ImGui::EndTooltip();
        }

    }

    // Window Extended Styles
    ImGui::TableSetColumnIndex(9);
    for (const auto& style : app.window_extendedstyles)
    {
        ImGui::Selectable(style.c_str());

        if (ImGui::IsItemClicked())
        {
            selected_row = i;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            PopupText(app);
            ImGui::EndTooltip();
        }
    }
}

void CreateHeader(const char* text)
{
    auto windowWidth = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(text).x) * 0.5f);
    ImGui::Text(text);
    ImGui::Spacing();
    ImGui::Separator();
}

void WindowFinder(HWND hwnd)
{
    // Get the dimensions of the CreateWindow window
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // Set the size and position of the ImGui window
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowPos(ImVec2(windowWidth / 2, windowHeight / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Window Finder", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    char searchTermChar[256];
    strcpy_s(searchTermChar, searchTerm.c_str());
    ImGui::PushItemWidth(250);
    ImGui::InputTextWithHint("##Search", "Search", searchTermChar, IM_ARRAYSIZE(searchTermChar));

    searchTerm = searchTermChar;

    ImGui::SameLine();

    if (ImGui::Button("Update Windows"))
    {
        windows.clear();
        EnumWindows(EnumWindowsProc, 0);
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
    {
        ImGui::SetTooltip("Press F8 to toggle update");
    }

    ImGui::SameLine();

    ImGui::Text("Total Windows: %d", windows.size());

    if (GetAsyncKeyState(VK_F8)) 
    {
        windows.clear();
        EnumWindows(EnumWindowsProc, 0);

    }

    ImGui::SameLine(ImGui::GetWindowWidth() - 84.0f);

    if (ImGui::Button("Settings"))
    {
        ImGui::OpenPopup("Settings");

    }
    ImGui::SetNextWindowSize(ImVec2(200, 185));
    if (ImGui::BeginPopup("Settings", ImGuiWindowFlags_NoResize))
    {
        CreateHeader("Settings");

        ImGui::Checkbox("Hide Child Windows", &hide_child);

        ImGui::EndPopup();
    }

    ImGui::Spacing();

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
        | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
        | ImGuiTableFlags_SizingFixedFit;

    if (IsWindowVisible(hwnd) && !IsIconic(hwnd))
    {
        ImGui::BeginTable("Data", 10, flags);
        ImVec2 initial_cursor_pos = ImGui::GetCursorPos();
        ImGui::TableSetupColumn("ID", 150);
        ImGui::TableSetupColumn("Window Name", 200);
        ImGui::TableSetupColumn("Class Name", 200);
        ImGui::TableSetupColumn("Process Name", 200);
        ImGui::TableSetupColumn("Process ID", 150);
        ImGui::TableSetupColumn("Thread ID", 150);
        ImGui::TableSetupColumn("HWND", 150);
        ImGui::TableSetupColumn("Window Size", 150);
        ImGui::TableSetupColumn("Window Styles", 200);
        ImGui::TableSetupColumn("Window Extended Styles", 200);
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableHeadersRow();

        if (searchTerm.empty())
        {
            for (int i = 0; i < windows.size(); i++)
            {
                const auto& app = windows[i];

                PrintWindows(app, i);
            }
        }
        else
        {
            std::string searchTermLower;
            std::transform(searchTerm.begin(), searchTerm.end(), std::back_inserter(searchTermLower), [](unsigned char c) { return std::tolower(c); });

            for (int i = 0; i < windows.size(); i++)
            {
                const auto& app = windows[i];

                std::string windowNameLower, windowClassLower, processNameLower, window_styles_lower, window_extendedstyles_lower, window_hwnd;

                std::ostringstream stream;
                stream << "0x" << std::hex << (int)app.hwnd;
                std::string hwnd_string = stream.str();

                std::transform(app.window_name.begin(), app.window_name.end(), std::back_inserter(windowNameLower), [](unsigned char c) { return std::tolower(c); });
                std::transform(app.window_class.begin(), app.window_class.end(), std::back_inserter(windowClassLower), [](unsigned char c) { return std::tolower(c); });
                std::transform(app.process_name.begin(), app.process_name.end(), std::back_inserter(processNameLower), [](unsigned char c) { return std::tolower(c); });

                std::transform(hwnd_string.begin(), hwnd_string.end(), std::back_inserter(window_hwnd), [](unsigned char c) { return std::tolower(c); });

                for (int j = 0; j < app.window_styles.size(); j++)
                {
                    std::transform(app.window_styles[j].begin(), app.window_styles[j].end(), std::back_inserter(window_styles_lower), [](unsigned char c) { return std::tolower(c); });
                }

                for (int j = 0; j < app.window_extendedstyles.size(); j++)
                {
                    std::transform(app.window_extendedstyles[j].begin(), app.window_extendedstyles[j].end(), std::back_inserter(window_extendedstyles_lower), [](unsigned char c) { return std::tolower(c); });
                }

                if (windowNameLower.find(searchTermLower) != std::string::npos
                    || windowClassLower.find(searchTermLower) != std::string::npos
                    || processNameLower.find(searchTermLower) != std::string::npos
                    || std::to_string(app.process_id).find(searchTermLower) != std::string::npos
                    || std::to_string(app.thread_id).find(searchTermLower) != std::string::npos
                    || window_hwnd.find(searchTermLower) != std::string::npos
                    || window_styles_lower.find(searchTermLower) != std::string::npos
                    || window_extendedstyles_lower.find(searchTermLower) != std::string::npos)
                {
                    PrintWindows(app, i);
                }
            }
        }
        ImGui::EndTable();

        if (selected_row != -1)
        {
            static ImGuiTableFlags flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;

            size_t length = strlen(windows[selected_row].window_name.c_str());

            if (length > 40)
            {
                flags = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
            }
            else
            {
                flags = 0;
            }

            ImGui::OpenPopup("Window Information");

            ImGui::SetNextWindowSize(ImVec2(600, 385));
            if (ImGui::BeginPopupModal("Window Information", NULL, ImGuiWindowFlags_NoResize))
            {
                ImGui::BeginChild("generalWin", ImVec2(400, 150), true, flags);
                {
                    CreateHeader("General");

                    ImGui::Text("Window ID: %i", selected_row);

                    std::string windowName = windows[selected_row].window_name.c_str();
                    if (windowName == "")
                    {
                        ImGui::Text("Window Name: N/A");
                    }
                    else
                    {
                        ImGui::Text("Window Name: %s", windows[selected_row].window_name.c_str());
                    }

                    ImGui::Text("Class Name: %s", windows[selected_row].window_class.c_str());
                    ImGui::Text("Process Name: %s", windows[selected_row].process_name.c_str());
                }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::BeginChild("otherWin", ImVec2(175, 150), true);
                {
                    CreateHeader("Other");

                    ImGui::Text("Process ID: %i", windows[selected_row].process_id);
                    ImGui::Text("Thread ID: %i", windows[selected_row].thread_id);

                    std::ostringstream stream;
                    stream << "0x" << std::hex << (int)windows[selected_row].hwnd;
                    std::string hwnd_string = stream.str();
                    ImGui::Text("HWND: %s", hwnd_string.c_str());

                    ImGui::Text("Size: %d x %d", windows[selected_row].rect.right - windows[selected_row].rect.left, windows[selected_row].rect.bottom - windows[selected_row].rect.top);
                }
                ImGui::EndChild();


                ImGui::BeginChild("windowStyles", ImVec2(280, 150), true);
                {
                    CreateHeader("Styles");

                    ImGui::BeginChild("styleData");
                    for (const auto& style : windows[selected_row].window_styles)
                    {
                        ImGui::Text("%s", style.c_str());
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::BeginChild("windowExtendedStyles", ImVec2(295, 150), true);
                {
                    CreateHeader("Extended Styles");

                    ImGui::BeginChild("exStyleData");
                    for (const auto& style : windows[selected_row].window_extendedstyles)
                    {
                        ImGui::Text("%s", style.c_str());
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                if (ImGui::Button("Back"))
                {
                    selected_row = -1;
                    ImGui::CloseCurrentPopup();

                }

                ImGui::SameLine();

                if (ImGui::Button("Copy To Clipboard"))
                {
                    std::ostringstream oss;

                    std::string windowN = windows[selected_row].window_name.c_str();
                    if (windowN == "")
                    {
                        oss << "Window Name: N/A \n";
                    }
                    else
                    {
                        oss << "Window Name: " << windows[selected_row].window_name << '\n';
                    }

                    oss << "Window Class: " << windows[selected_row].window_class << '\n';
                    oss << "Process Name: " << windows[selected_row].process_name << '\n';
                    oss << "Window HWND: 0x" << std::hex << (int)windows[selected_row].hwnd << '\n';
                    oss << "Process ID: " << std::dec << windows[selected_row].process_id << '\n';
                    oss << "Thread ID: " << std::dec << windows[selected_row].thread_id << '\n';
                    oss << "Window Size: " << std::dec << windows[selected_row].rect.right - windows[selected_row].rect.left << " x " << windows[selected_row].rect.bottom - windows[selected_row].rect.top << "\n";

                    oss << "Window Styles: ";
                    for (const auto& style : windows[selected_row].window_styles)
                    {
                        oss << style << " ";
                    }
                    oss << '\n';

                    oss << "Window Extended Styles: ";
                    for (const auto& extendedstyle : windows[selected_row].window_extendedstyles)
                    {
                        oss << extendedstyle << " " << "\n";
                    }

                    //RECT windowRect;
                    //GetWindowRect(windows[selected_row].hwnd, &windowRect);
                    //oss << "Window Position: (" << windowRect.left << ", " << windowRect.top << "), (" << windowRect.right << ", " << windowRect.bottom << ")\n";
                    //oss << "Window Handle: " << windows[selected_row].hwnd << '\n';
                    //oss << "Parent Window: " << GetParent(windows[selected_row].hwnd) << '\n';
                    //oss << "Window Menu: " << GetMenu(windows[selected_row].hwnd) << '\n';
                    //oss << "Window Title: " << GetWindowTextW(windows[selected_row].hwnd) << '\n';

                    std::string clipboard_content = oss.str();

                    if (OpenClipboard(NULL))
                    {
                        EmptyClipboard();
                        HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, clipboard_content.size() + 1);
                        if (hClipboardData != NULL)
                        {
                            char* pchData = (char*)GlobalLock(hClipboardData);
                            strcpy_s(pchData, clipboard_content.size() + 1, clipboard_content.c_str());
                            GlobalUnlock(hClipboardData);
                            SetClipboardData(CF_TEXT, hClipboardData);
                        }
                        CloseClipboard();
                    }
                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Clipboard might not have enough capacity to store all the data.");
                    ImGui::EndTooltip();
                }

                ImGui::SameLine();

                if (ImGui::Button("Save To File"))
                {
                    std::ostringstream oss;

                    std::string windowN = windows[selected_row].window_name.c_str();
                    if (windowN == "")
                    {
                        oss << "Window Name: N/A \n";
                    }
                    else
                    {
                        oss << "Window Name: " << windows[selected_row].window_name << '\n';
                    }

                    oss << "Window Class: "  << windows[selected_row].window_class << '\n';
                    oss << "Process Name: " << windows[selected_row].process_name << '\n';
                    oss << "Window HWND: 0x" << std::hex << (int)windows[selected_row].hwnd << '\n';
                    oss << "Process ID: " << std::dec << windows[selected_row].process_id << '\n';
                    oss << "Thread ID: " << std::dec << windows[selected_row].thread_id << '\n';
                    oss << "Window Size: " << windows[selected_row].rect.right - windows[selected_row].rect.left << " x " << windows[selected_row].rect.bottom - windows[selected_row].rect.top << "\n";

                    oss << "Window Styles: ";
                    for (const auto& style : windows[selected_row].window_styles)
                    {
                        oss << style << " ";
                    }
                    oss << '\n';

                    oss << "Window Extended Styles: ";
                    for (const auto& extendedstyle : windows[selected_row].window_extendedstyles)
                    {
                        oss << extendedstyle << " " << "\n";
                    }

                    std::string file_content = oss.str();

                    wchar_t fileName[MAX_PATH];
                    OPENFILENAME ofn;
                    ZeroMemory(fileName, sizeof(fileName));
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0Any File (*.*)\0*.*\0";
                    ofn.lpstrFile = fileName;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrTitle = L"Save All Info";
                    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                    ofn.lpstrDefExt = L"txt";
                    if (GetSaveFileNameW(&ofn))
                    {
                        std::wofstream outFile(fileName);
                        outFile << file_content.c_str();
                        outFile.close();
                    }
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}
