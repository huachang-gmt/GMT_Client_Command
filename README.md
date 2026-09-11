# GMT CLIENT Command Line Interface (Windows 11 平台 執行)

## 程式所在位置 ： D:\RaspberryPi\GMT_Client_Command\

## 編譯工具下載：
- CMake ： https://cmake.org/download/   下載 Binary distributions: 的 Windows x64 Installer: 下載完成後安裝。

- Visual Studio 工具 ：  https://visualstudio.microsoft.com/zh-hant/downloads/   網頁往下捲動，看到 『Visual Studio 工具 』 選擇 『適用於 Visual Studio 2026 的建置工具 』 按下 『下載』。 下載完成後，執行安裝。 安裝完成後會生成 編譯視窗，很像 一般 Terminal 視窗，編譯必須在此視窗內執行。 安裝完成後，可以從 Windows 開始 找到 Visual Studio 資料夾，點擊打開，就會出現 『Develop Powershell for VS』 視窗。

## 編譯方式 

```text
1. 先開啟專屬 Terminal ： Developer Command Prompt for VS 。 執行位置在 C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio\Visual Studio Tools 下 。

2. PS D:\RaspberryPi\GMT_Client_Command> cmake -S . -B build  << 編譯一

3. PS D:\RaspberryPi\GMT_Client_Command> cmake --build build  << 編譯二

4. 執行 ： PS D:\RaspberryPi\GMT_Client_Command> .\build\Debug\GMT_Client_Command.exe
```

## 執行順序

- Raspberry Pi CM5 的 TCP Server 必須先執行 。

```text
herman@RPiCM5:~/Gmt_CMD_Parser/build $ ./GMT_Server_Command

========================================
GMT Server Command Test
========================================
[OK] socket() created. fd=3
[OK] bind() successful. port=9999
[OK] listen() successful.
[WAIT] Waiting for client connection...
```
- 再執行 電腦端的執行檔案 GMT_Client_Command.exe

```text
PS D:\RaspberryPi\GMT_Client_Command> .\build\Debug\GMT_Client_Command.exe

[CONNECT] 192.168.137.200:9999
[OK] Connected to TCP Server.
========================================
GMT Client Command
========================================
0.  Quit
1.  INS 1
2.  STP
3.  SAH M01 M02 M03
4.  SHC M01 2 17 400000 40000 0 128
5.  SHC? M01
6.  SVO
7.  SVF
8.  CAL
9.  DSC
10. VLS 0.15
11. MOV R 2000 2000 1000 0 0.5 1
12. MRV R 2000 2000 1000 0 0.5 1
13. MSV M02 2000
14. MSR M06 1
15. MPV M01 M03 1200.0 -35.5
16. MPR M01 M03 1200.0 -35.5
========================================
GMT>
```
- 在 GMT> 下可以輸入 上方選單的編號，也可以直接輸入 命令，例如：

```text
GMT> SHC M01 2 17 400000 40000 0 128
輸出結果 ： 
[TX] SHC M01 2 17 400000 40000 0 128
[RX] VALID
```

# GMT_Client_Command

Windows 端 GMT 指令列 TCP Client。

`GMT_Client_Command` 是 GMT Command Parser 系統中的 End User 端測試程式，用於模擬最終使用者輸入 GMT 命令，透過 TCP 將命令送往 Raspberry Pi CM5，再由 CM5 端的 Command Parser 進行命令格式與參數驗證。

目前本專案主要用途為：

* Windows End User CLI 測試
* TCP 通訊測試
* CM5 Command Parser 整合測試
* GMT Command 測試
* VALID / INVALID 結果驗證
* 後續正式 End User Client 的開發基礎

---

# 1. 系統整體架構

GMT 系統的目標架構如下：

```text
End User
   │
   │ CLI Command
   ▼
GMT_Client_Command
   │
   │ TCP :9999
   ▼
Raspberry Pi CM5
   │
   ├── TCP Server
   │
   └── Command Parser
          │
          ├── VALID
          │
          └── INVALID
   │
   │ VALID Command
   ▼
USB CDC Transport
   │
   ▼
STM32H755
   │
   ▼
SPI1 / EtherCAT
   │
   ▼
Motion Drivers
```

目前 `GMT_Client_Command` 已經完成並驗證的部分為：

```text
GMT_Client_Command
        │
        │ TCP
        ▼
CM5 GMT_Server_Command
        │
        ▼
CommandParser
```

目前尚未在本專案中整合：

* USB CDC Transport
* STM32H755
* SPI1
* EtherCAT
* Motion Controller

這些功能將在後續階段逐步整合。

---

# 2. 專案目的

本專案主要提供 End User 端的 CLI 操作介面。

目前功能包括：

1. 建立與 CM5 TCP Server 的 TCP 連線。
2. 透過 CLI 輸入 GMT Command。
3. 提供預先建立的 Command Menu。
4. 支援直接輸入任意 Command。
5. 將 Command 傳送至 CM5。
6. 接收 CM5 Command Parser 的結果。
7. 顯示 `VALID` 或 `INVALID`。
8. 支援同一個 TCP 連線連續測試多個 Command。
9. 支援無效 Command 測試。
10. 作為未來正式 End User Client 的開發基礎。

---

# 3. 開發環境

## 3.1 作業系統

目前開發與測試環境：

```text
Windows 11
```

---

## 3.2 C++ 編譯器

本專案使用 Microsoft Visual C++。

目前實際使用：

```text
Visual Studio Build Tools 18
MSVC 19.51.36257.0
```

本專案不要求一定安裝完整 Visual Studio IDE。

使用 Visual Studio Build Tools 即可。

---

## 3.3 CMake

目前使用：

```text
CMake 4.4.3
```

CMake 用於產生 Visual Studio 建置專案。

---

# 4. 環境架設

第一次在新的 Windows 電腦建立本專案時，需要準備：

1. Visual Studio Build Tools
2. MSVC C++ 編譯工具
3. Windows SDK
4. CMake

安裝完成後，應該可以使用：

```text
cl.exe
cmake.exe
```

---

## 4.1 確認 MSVC

開啟：

```text
Developer PowerShell for Visual Studio
```

執行：

```powershell
cl
```

如果可以看到 Microsoft C/C++ Compiler 的版本資訊，代表 MSVC 環境正常。

目前開發環境確認版本：

```text
MSVC 19.51.36257.0
```

---

## 4.2 確認 CMake

執行：

```powershell
cmake --version
```

目前開發環境確認：

```text
CMake 4.4.3
```

---

# 5. 專案目錄

目前專案位於：

```text
D:\RaspberryPi\GMT_Client_Command
```

目前專案結構：

```text
GMT_Client_Command/
├── CMakeLists.txt
├── README.md
└── src/
    └── main.cpp
```

目前程式核心全部位於：

```text
src/main.cpp
```

目前專案刻意保持簡單，不加入不必要的第三方函式庫。

---

# 6. CMake 設定

本專案使用 C++17。

目前 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.16)

project(GMT_Client_Command
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(GMT_Client_Command
    src/main.cpp
)
```

目前沒有使用第三方函式庫。

Windows Socket 通訊使用 Windows 系統提供的 Winsock API。

---

# 7. 建立專案

開啟 Visual Studio Developer PowerShell。

進入專案：

```powershell
cd D:\RaspberryPi\GMT_Client_Command
```

第一次建立：

```powershell
cmake -S . -B build
```

進行編譯：

```powershell
cmake --build build
```

如果要明確建立 Debug 版本：

```powershell
cmake --build build --config Debug
```

---

# 8. 編譯後執行檔

成功編譯後，Debug 執行檔位於：

```text
build\Debug\GMT_Client_Command.exe
```

目前實際路徑：

```text
D:\RaspberryPi\GMT_Client_Command\build\Debug\GMT_Client_Command.exe
```

執行：

```powershell
.\build\Debug\GMT_Client_Command.exe
```

---

# 9. TCP 通訊設定

目前開發測試使用的 CM5 IP：

```text
192.168.137.200
```

Command TCP Port：

```text
9999
```

因此目前 TCP Server 位址：

```text
192.168.137.200:9999
```

本專案目前連線設定為：

```text
Server IP   = 192.168.137.200
Server Port = 9999
```

注意：

```text
8888
```

是另一個 Routine Status 通訊用途，不是 Command TCP Port。

---

# 10. Client 程式架構

目前 Client 的工作流程：

```text
程式啟動
   │
   ▼
初始化 Windows Socket
   │
   ▼
連線 CM5 TCP Server
   │
   ▼
顯示 Command Menu
   │
   ▼
等待使用者輸入
   │
   ├── 輸入 Menu 編號
   │       │
   │       ▼
   │   取得對應 Command
   │
   ├── 輸入完整 Command
   │       │
   │       ▼
   │   直接傳送
   │
   └── 輸入 0 / exit
           │
           ▼
         離開
```

目前 TCP 連線會保持開啟。

因此使用者可以在同一個 TCP Connection 中連續測試多個 Command。

---

# 11. Command 傳輸方式

Client 使用 TCP 將 Command 傳送給 CM5。

每一個 Command 最後會加入：

```text
CRLF
```

也就是：

```text
\r\n
```

例如：

```text
STP\r\n
```

或：

```text
MOV R 2000 2000 1000 0 0.5 1\r\n
```

Client 本身不負責判斷 Command 是否正確。

Command 的格式與參數驗證由 CM5 的 Command Parser 負責。

---

# 12. CLI Command Menu

目前已建立 25 個已驗證的 Command / Manual Example：

```text
1.  INS 1
2.  STP
3.  SAH M01 M02 M03
4.  SHC M01 2 17 400000 40000 0 128
5.  SHC? M01
6.  SVO
7.  SVF
8.  CAL
9.  DSC
10. VLS 0.15
11. MOV R 2000 2000 1000 0 0.5 1
12. MRV R 2000 2000 1000 0 0.5 1
13. MSV M02 2000
14. MSR M06 1
15. MPV M01 M03 1200.0 -35.5
16. MPR M01 M03 1200.0 -35.5
17. MOV?
18. POS?
19. PMS?
20. SPI R U 10 5 2
21. SPI?
22. FRS?
23. DFRS ScanRoutine01
24. FLM M10 0.2 V 0.01 TH 255
25. BKN?
```

這些 Command 都已經透過：

```text
GMT_Client_Command
        ↓
TCP
        ↓
GMT_Server_Command
        ↓
CommandParser
```

進行實際測試。

---

# 13. Menu 編號與 Command

Menu 編號只是一個 Client 端的方便操作方式。

例如：

```text
GMT> 15
```

Client 會轉換成：

```text
MPV M01 M03 1200.0 -35.5
```

然後送出：

```text
MPV M01 M03 1200.0 -35.5\r\n
```

因此：

```text
Menu 編號
   ↓
Client 取得 Command
   ↓
TCP 傳送 Command
   ↓
CM5 Parser
```

---

# 14. 自由輸入 Command

除了 Menu 之外，Client 也支援直接輸入 Command。

例如：

```text
GMT> MPV M01 M03 1200.0 -35.5
```

Client 會直接送出：

```text
MPV M01 M03 1200.0 -35.5
```

Parser 回應：

```text
[RX] VALID
```

這個功能非常重要，因為可以測試尚未加入 Menu 的 Command，也可以測試錯誤 Command。

例如：

```text
GMT> UNKNOWN
```

結果：

```text
[TX] UNKNOWN
[RX] INVALID
```

---

# 15. 與 CM5 Server 搭配

目前 Windows Client 測試使用：

```text
GMT_Server_Command
```

這是一個位於 CM5 的測試用 TCP Server。

CM5 專案：

```text
~/Gmt_CMD_Parser
```

目前測試程式：

```text
src/GMT_Server_Command.cpp
```

編譯後：

```text
build/GMT_Server_Command
```

---

# 16. 啟動 CM5 測試 Server

在 CM5：

```bash
cd ~/Gmt_CMD_Parser/build
./GMT_Server_Command
```

正常啟動後：

```text
========================================
GMT Server Command Test
========================================
[OK] socket() created. fd=3
[OK] bind() successful. port=9999
[OK] listen() successful.
[WAIT] Waiting for client connection...
```

等待 Windows Client 連線。

---

# 17. 啟動 Windows Client

在 Windows Developer PowerShell：

```powershell
cd D:\RaspberryPi\GMT_Client_Command
.\build\Debug\GMT_Client_Command.exe
```

成功連線後：

```text
[CONNECT] 192.168.137.200:9999
[OK] Connected to TCP Server.
```

然後顯示：

```text
========================================
GMT Client Command
========================================
...
GMT>
```

---

# 18. 實際測試範例

例如輸入：

```text
GMT> 1
```

Client：

```text
[TX] INS 1
[RX] VALID
```

輸入：

```text
GMT> 15
```

Client：

```text
[TX] MPV M01 M03 1200.0 -35.5
[RX] VALID
```

也可以直接輸入：

```text
GMT> MPV M01 M03 1200.0 -35.5
```

結果：

```text
[TX] MPV M01 M03 1200.0 -35.5
[RX] VALID
```

---

# 19. INVALID Command 測試

直接輸入不存在的 Command：

```text
GMT> UNKNOWN
```

結果：

```text
[TX] UNKNOWN
[RX] INVALID
```

CM5 端：

```text
[RX] UNKNOWN
[Parser] INVALID - Unknown command
[TX] INVALID
```

代表 Command 已經成功經過：

```text
Windows Client
    ↓
TCP
    ↓
CM5
    ↓
CommandParser
    ↓
INVALID
    ↓
TCP Response
    ↓
Windows Client
```

---

# 20. TCP 長連線測試

目前 Client 不會每送一個 Command 就重新建立 TCP 連線。

例如：

```text
GMT> 1
[TX] INS 1
[RX] VALID

GMT> 2
[TX] STP
[RX] VALID

GMT> 3
[TX] SAH M01 M02 M03
[RX] VALID

GMT> UNKNOWN
[TX] UNKNOWN
[RX] INVALID
```

以上 Command 都是在同一個 TCP Connection 中完成。

測試完成後：

```text
GMT> 0
```

Client 離開。

CM5 Server 會收到：

```text
[INFO] Client disconnected.
```

---

# 21. 目前測試結果

目前已完成的測試：

```text
TCP 連線                         PASS
TCP Port 9999                    PASS
Windows → CM5 通訊               PASS
TCP 長連線                        PASS
CLI Menu                         PASS
Menu 編號 → Command              PASS
自由輸入 Command                  PASS
CRLF Command 傳送                 PASS
VALID Response                    PASS
INVALID Response                  PASS
Client Disconnect                 PASS
Parser 整合測試                   PASS
```

目前已實際驗證：

```text
25 個 Command / Manual Examples
```

測試路徑：

```text
GMT_Client_Command
        ↓
       TCP
        ↓
GMT_Server_Command
        ↓
 CommandParser
```

目前結果全部通過。

---

# 22. 目前開發 Checkpoint

目前專案進度：

```text
================================================
GMT_Client_Command
Windows End User CLI TCP Client
================================================

Windows 11                         完成
MSVC / C++17                       完成
CMake 建置                         完成
TCP Client                         完成
TCP Port 9999                      完成
CLI Menu                           完成
自由輸入                           完成
TCP 長連線                         完成
Parser 整合測試                    完成
25 個 Command 測試                 PASS
VALID / INVALID 測試               PASS
================================================
```

這個 Checkpoint 代表：

**Windows End User CLI Client → CM5 Command Parser**

已經完成第一階段的端到端驗證。

---

# 23. 目前尚未完成的工作

本專案目前並不是最終完成版本。

後續仍有以下工作。

## 23.1 完成全部 GMT Command

目前目標為：

```text
約 36 個 GMT Command
```

目前：

```text
25 個已確認並測試
```

剩餘 Command 必須繼續依照正式 Command 文件確認。

原則：

> 文件沒有提供明確格式或範例時，不自行猜測 Command 的參數格式。

---

## 23.2 完成 Command Parser

CM5 的 Command Parser 必須逐步完成所有正式支援的 Command。

Parser 負責：

```text
Command 名稱
    ↓
Syntax Validation
    ↓
Parameter Validation
    ↓
VALID / INVALID
```

Windows Client 不應該複製這些 Parser 規則。

---

## 23.3 將測試成功內容整合到正式 TCP Server

目前：

```text
GMT_Server_Command
```

只是測試用 Server。

正式架構仍然是：

```text
Gmt_CMD_Parser
    │
    ├── tcp_server.cpp
    ├── command_parser.cpp
    └── main.cpp
```

等測試階段完成後，成功驗證的 TCP → Parser 邏輯應整合回正式 `TcpServer`。

不應讓測試程式永久取代正式 Server。

---

# 24. 後續 USB Transport

Parser 完成後，正式架構下一階段：

```text
Windows Client
      │
      │ TCP
      ▼
CM5 TCP Server
      │
      ▼
Command Parser
      │
      ├── INVALID
      │      │
      │      ▼
      │   TCP Error
      │
      └── VALID
             │
             ▼
        USB Transport
             │
             ▼
          STM32H755
```

重要原則：

```text
Command Parser
```

負責命令驗證。

```text
USB Transport
```

負責傳輸。

兩者不應混在一起。

---

# 25. STM32H755 後續架構

USB Transport 接通後，命令將進一步進入 STM32H755：

```text
CM5
 │
 │ USB CDC
 ▼
STM32H755
 │
 ▼
Command Execution
 │
 ▼
SPI1 / EtherCAT
 │
 ▼
Motion Controller
```

EtherCAT 與 Motion Control 的實際演算法屬於另一個功能模組，不由本 Windows Client 負責。

---

# 26. 後續 Response 回傳

最終完整的 Command Response 流程預計為：

```text
End User
   │
   ▼
GMT_Client_Command
   │
   │ TCP
   ▼
CM5
   │
   │ USB
   ▼
STM32H755
   │
   ▼
Motion / EtherCAT
   │
   │ Execution Result
   ▼
STM32H755
   │
   ▼
CM5
   │
   ▼
GMT_Client_Command
   │
   ▼
End User
```

目前 Windows Client 已經可以接收並顯示 CM5 Server 回應。

未來只需要隨著正式 Response Protocol 的定義進一步擴充。

---

# 27. 未來可能增加的 Client 功能

目前 Client 先維持簡單。

未來可以依需求增加：

* Command History
* CLI Help
* Command 說明
* TCP 連線狀態
* TCP 斷線自動重連
* Connection Timeout
* Server Error 顯示
* Response 類型分類
* Command 測試模式
* 自動化 Regression Test
* 完整 Command Menu
* Server IP 設定
* TCP Port 設定

這些功能應該在目前 Parser 與 TCP 正式架構穩定後再逐步加入。

---

# 28. GitHub Repository

本專案為獨立 GitHub Repository：

```text
GMT_Client_Command
```

Repository 建議至少包含：

```text
GMT_Client_Command/
├── CMakeLists.txt
├── README.md
└── src/
    └── main.cpp
```

編譯產生的 `build` 目錄不應提交到 GitHub。

建議 `.gitignore`：

```text
build/
.vs/
*.vcxproj.user
```

---

# 29. 開發方式

本專案採取逐階段驗證方式：

```text
修改
  ↓
編譯
  ↓
單項測試
  ↓
整合測試
  ↓
確認結果
  ↓
Checkpoint
  ↓
GitHub
  ↓
下一階段
```

避免一次修改多個模組，造成問題難以定位。

測試程式可以在開發階段使用，但經過驗證的功能最終必須整合回正式架構。

---

# 30. 目前版本定位

目前 `GMT_Client_Command` 屬於：

```text
Development / Integration Test Client
```

目前主要任務：

```text
Windows End User CLI
        ↓
TCP
        ↓
CM5 Command Parser
```

它目前不是最終正式的 GUI 或完整產品 Client。

隨著整體 GMT 系統逐步完成，本專案將繼續擴充。

---

# 31. 開發 Roadmap

目前規劃：

```text
[完成]
Windows CLI Client
        │
        ▼
[完成]
TCP :9999
        │
        ▼
[完成]
CM5 Command Parser 整合測試
        │
        ▼
[進行中]
36 個 Command 完整性確認
        │
        ▼
[下一階段]
正式 TcpServer 整合
        │
        ▼
[後續]
VALID → USB Transport
        │
        ▼
[後續]
STM32H755
        │
        ▼
[後續]
EtherCAT / Motion
        │
        ▼
[後續]
完整 Command Response
```

---

# 32. 今日 Checkpoint 總結

截至目前：

```text
GMT_Client_Command
```

已經成功完成：

```text
Windows CLI Client
        ↓
TCP 192.168.137.200:9999
        ↓
CM5 GMT_Server_Command
        ↓
CommandParser
```

並完成：

```text
25 個已確認 Command / Manual Examples
```

的實際整合測試。

VALID 與 INVALID 流程皆已確認正常。

這個 Checkpoint 可以作為後續繼續開發 GMT Command Parser、正式 TCP Server、USB Transport 以及 STM32H755 整合的基礎。

---

# 33. 後續開發原則

後續開發遵循以下原則：

1. 不任意猜測 Command 文件沒有定義的參數格式。
2. Parser 與 TCP Client 保持分離。
3. TCP Transport 與 Command Validation 保持分離。
4. 測試成功後再整合回正式程式。
5. 每完成一個階段先 Build。
6. Build 通過後進行實機測試。
7. 測試全部通過後建立 GitHub Checkpoint。
8. 再進入下一個開發階段。

---
