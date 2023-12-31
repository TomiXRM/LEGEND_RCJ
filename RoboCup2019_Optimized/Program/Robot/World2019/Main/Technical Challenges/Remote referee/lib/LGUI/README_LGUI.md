Team LEGEND UI API library 
# LGUI.h Reference

UI用のライブラリのリファレンスです。

## アーキテクチャ
Serialクラスのインスタンス `device`を基底にもつクラス `LGUI` による構成です。

あらゆるモード管理の処理と、モード変更/選択されたモードの受信/実行に必要な処理は、`LGUI`クラスのメンバとして含まれています。

具体的には、
- モード管理テーブル
- 各モードで使用する関数のテーブル

をメンバ変数として保有し、
- モードの受信
- モードの実行
- モード切り替え

の処理をメンバ関数として保有しています。

したがって、親となるプロセスから、モード管理の処理をトリガーするメソッド `LGUI::Run()`を実行すれば、その度にモードを確認し、先に`Entry(char c,Callback<void()> before, Callback<void()>loop,Callback<void()> after);` メソッドによって登録されたモードから検索して対応する処理を実行します。

なお、モード変更に使用する`MODESW`モードは、**特殊モードとして実装されているわけではなく、コンストラクタから`Entry`をコールすることによって定義される一つのモードとして実装されています。** 。誤解なきようご注意ください。

## 使い方

### 定義
```cpp
LGUI MyLGUI(&pc,PA_9,PA_10);

```
 新しいインスタンスを定義するには、LGUIクラスを初期化します。
 引数は以下の通りです。

Argument|Type|Description
 |---|---|---|
 |_pc|RawSerial*|デバッグ情報が出力されるシリアルポートです。|
 |tx|PinName|実際のLGUIボードとの通信に使用するシリアル出力ピンです。|
 |rx|PinName|実際のLGUIボードとの通信に使用するシリアル入力ピンです。|

*なお、tx,rxによって指定されたピンは、内部的に基底となっている`mbed:Serial`オブジェクトのコンストラクタ引数として利用されます。したがって、ここで指定されたポートはそのオブジェクトに排他的に制御されるため、他の目的と兼用することはできません。*


### 通信の設定

```cpp

MyLGUI.baud(115200);

```

これだけです。簡単でしょ?


### モードの追加

```cpp

MyLGUI.Entry('P',DONOTHING, callback(&Body_process),DONOTHING);

```

モードをテーブルに追加するには、`Entry` メソッドを実行します。
引数は以下の通りです。

|Argument|type|Description|
|---|---|---|
|c|char|モードフラグです。|
|before|Callback<void()>|そのモードに入る前に一度実行される関数を指定。|
|loop|Callback<void()>|そのモードの途中で`LGUI::Run()`が実行される時何回でも実行される関数の指定です。処理の本体はここに書くことが推奨されます。|
|after|Callback<void()>|そのモードから離脱するときに実行される関数の指定です。|

*ただし、それぞれの引数に該当する処理のないときは、引数に`DONOTHING`を指定することでその旨を明示できます。そのためだけに空の関数を作るなどのマナー違反はやめてください。事故の元です。*
