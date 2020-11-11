# VariantDetect

[English](#About)/[日本語](#このプログラムについて)

## About
This program detects variants from text format sequence or Sanger sequence result (.ab1).  

## Install
Please see the repository top.

## Usage
_I. For users who execute the program on your computer._
1. Launch the CUI software. 
- Mac/Linux/Unix  
Launch the "Terminal" application.  
- Windows  
Launch the "Command Prompt".  

2. Move to the directory, in which the GenomeExtract was installed.

3. Exec the command.

### Example
> ./VariantDetect -r {SOMEWHERE}/human.bin -i {SOMEWHERE}/test.ab1

_II. For users who execute the program on google colab._
1. Open the [template notebook](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) on your Google colab.  

2. Install the required libraries and GenomeExtract according to the guidance in the notebook.

3. Upload the GFF3 format annotation file to Google colab temporal storage and execute this program.

***

## このプログラムについて
このプログラムは、テキストベースのシーケンスファイルや、サンガーシーケンスの波形データを含むファイル（.ab1）から、変異を探し出すソフトです。

## インストール方法
インストール方法はリポジトリのトップページを参照してください。

## 使用法  
_I. 自分のコンピュータ上で実行する場合_
1. 
- Mac/Linux/Unixユーザ  
「ターミナル」アプリを起動してください。  
- Windows  
「コマンドプロンプト」アプリを起動してください。    

2. インストールしたソフトウェアのあるディレクトリに移動します。

3. コマンドを入力して実行します。

### 例 (ヒト)  
> ./VariantDetect -r {SOMEWHERE}/human.bin -i {SOMEWHERE}/test.ab1

_II. Google Colab.上で実行する場合_
1. [テンプレートノートブック](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template_ja.ipynb) をGoogle colab.に読み込みます。  

2. ノートブック内のガイダンスに従って、必要なライブラリとアプリケーション本体をインストールしてください。

3. 変換したいFASTAシーケンスファイルを入手して、Google colab.上のストレージに保存した後、プログラムを実行します。
