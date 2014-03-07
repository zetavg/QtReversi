QtReversi
=========

電腦圖學導論實習的第一個作業，黑白棋遊戲程式。<del>好像限定要用 C++，我不會寫 C 啊</del>

雖然有現成的範例可以套，但需要使用微軟的 Visual Studio，我卻比較習慣 [Unix 的邏輯](http://gogojimmy.net/2012/04/07/why-programmer-should-use-mac/) (干這啥事)，再加上不喜歡被單一平台綁住 (Unix 有一堆選擇，Windows 就只有 Windows...)，而且 (聽說) 以後也會用到 [Qt](http://qt-project.org/)，乾脆現在就先拿來玩看看。<del>絕不是懶得習慣 Visual Studio</del>

## Compile

### by command line

需要：Qt `qmake`

產生 Makefile

```
qmake
```

編譯

```
make
```

清理

```
make distclean
```

### by Qt Creator

直接在 Qt Creator 開啓 QtReversi.pro，按 `建置專案` ...

#### [Windows] Missing Qt5Core.dll?

在這裡找到了答案: [http://stackoverflow.com/questions/16022967/run-exe-without-qt](http://stackoverflow.com/questions/16022967/run-exe-without-qt)

在 windows 上編譯出來的 .exe 一般不會把 Qt 一起編譯進去，所以需要把 Qt 相關的 .dll 檔擺好，一起拿到其他電腦上才能執行。

1. 將 “C:\Qt\Qt5.2.1\5.2.1\mingw48_32\bin” 底下的 .dll 複製到 .exe 相同的目錄。  
*d.dll 結尾的 .dll 是除錯用的，不需要複製。
2. 將 “C:\Qt\Qt5.2.1\5.2.1\mingw48_32\plugins\platforms” 整個資料夾複製到與 .exe 相同的目錄。
3. 應該就可以了 :p


## 架構

### 檔案

```
QtReversi
|-- QtReversi.pro      -- Qt 專案檔
|-- main.cpp           -- 主程式
|-- game.h             -- Game (遊戲控制) 物件
|-- img.qrc            -- 圖片資源庫
`-- img/               -- 圖片
    |-- stoneW.png     -- 白棋圖片
    |-- stoneB.png     -- 黑棋圖片
    |-- stoneWa.png    -- 半透明白棋圖片
    |-- stoneBa.png    -- 半透明黑棋圖片
    |-- stoneWs.png    -- 小白棋圖片
    |-- stoneBs.png    -- 小黑棋圖片
    `-- null.png       -- 空白圖片
```


### 第一次 Qt <del>就上手</del>

開發環境：OS X 10.9.1 + Qt 5.2

看 [http://openhome.cc/Gossip/Qt4Gossip/](http://openhome.cc/Gossip/Qt4Gossip/)

- 先[到官網下載](http://qt-project.org/downloads) Qt 來安裝 Qt libraries + Qt Creator (Mac 的安裝檔竟然最小，什麼邏輯) (其實只要裝 Qt libraries 就好，Qt Creator 裝了我大概也不會去用它 Orz)
- 打開小黑黑視窗 (terminal)，要使用 qmake 等 command line 工具，必須先將 Qt 的 /bin 加入 PATH，例如: `PATH="$HOME/Qt5.2.1/5.2.1/clang_64/bin:$PATH"`
- 開一個新的目錄當專案目錄，建立 hello.cpp 並加入以下內容：

```
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
     QApplication app(argc, argv);

     QLabel *label = new QLabel("Hello!World! Orz...");
     label->setWindowTitle("First Qt!");
     label->resize(200, 50);
     label->show();

     return app.exec();
}
```

- 回到 terminal，`qmake -project` 自動產生 .pro 專案檔，這個動作只要做一次
- 編輯這個 .pro，加入：

```
TEMPLATE += app
QT += gui declarative
```
([http://stackoverflow.com/questions/8995399/error-qapplication-no-such-file-or-directory](http://stackoverflow.com/questions/8995399/error-qapplication-no-such-file-or-directory))

- 以 .pro 專案檔的內容自動產生 Makefile: `qmake` (每次更新 .pro 的時候就需要再做一次 `qmake`)
- `make`

- 若要將圖片包進 App，需要先將圖片加入 RESOURCES。在 .pro 中加入 `RESOURCES += qre.qrc`，qre.qrc 內容如下：

```
<RCC>
	<qresource prefix="/">
		<file>foo.png</file>
		<file>bar.png</file>
	</qresource>
</RCC>
```

然後就可以直接在程式中用 `":/foo.png"` 來取得圖片。例如：

```
QPixmap *img = new QPixmap(":/bar.png"); 
```
