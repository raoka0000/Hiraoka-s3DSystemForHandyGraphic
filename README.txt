Hiraoka's3D_Systemについて。

・本プログラムの概要。
　本プログラムはHandyGraphicだけを用いて３D描写するシステムです。
 目指すのはアニメーションではなくシステムですので、本プログラムを利用し誰でも３D描写をできるように関数を利用するだけで描写ができることを意識しました。
 今回提出する作品ではとてもシンプルな立体の描写とアクションになっていますが、手間を掛ければ複雑なアニメーションも可能です。
　立体のデータは付属のtxtファイルから読み込む形式になっています。

・操作方法
　<最初にしなければならない操作>
最初に二つ画面が出てきますので左側の画面をクリックしてください。

<イベント>
‘q’ -> プログラムを終了します。
‘c’ -> 立体を回転させます。押すごとに回転する立体が変わります。

<左カメラ移動>
‘十字キー’ ->　それぞれ最初の画面においての前横の移動となります。
‘z , x’ -> 上下の移動です。
‘w,s,a,d’ -> カメラの向きを操作します。

<右カメラ移動>
‘1,2,3,4’ ->　それぞれ最初の画面においての前横の移動となります。
‘5,6’ -> 上下の移動です。
‘7,8,9,0’ -> カメラの向きを操作します。


・注意書きみたいなもの
カメラと移動の方向がかみ合っていません。
　HandyGraphicの機能を使い影のシェーダを作ることが難しいと判断した為、影の実装はできていません。
なので本作品は球の表現はできません。また、カメラが立体に入り込んだ時の描写の挙動がおかしくなります。
　本作では前学期のプログラムを一部流用しています。そのため、作り始めと作り終わりで私自身の能力が大きく違いますので、プラグラムが統一感のないものになってます。



