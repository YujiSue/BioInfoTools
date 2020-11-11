# GFFParser

[English](#About)/[日本語](#このプログラムについて)

## About
This program extracts "source", "type", and keys of "attributes" from GFF3 format file.  
For a description of the GFF3 file format, please see the [GMOD's wiki](http://gmod.org/wiki/GFF3).  

## Install
Please see the repository top.

## Usage
_I. For users who execute the program on your computer._
1. Download the GFF3 format annotation file from database site such as [Ensembl](http://m.ensembl.org/info/data/ftp/index.html).  
Nematode researchers can obtain gff3 file from [wormbase ftp site](ftp://ftp.wormbase.org/pub/wormbase/species/).

2. Launch the CUI software. 
- Mac/Linux/Unix  
Launch the "Terminal" application.  
- Windows  
Launch the "Command Prompt".  

3. Move to the directory, in which the GenomeConverter was installed.

4. Exec the command.

### Example (human)  
> $ ./GFFParser {SOMEWHERE}/Homo_sapiens.GRCh38.99.chr.gff3    


_II. For users who execute the program on google colab._
1. Open the [template notebook](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) on your Google colab.  

2. Install the required libraries and GFFParser according to the guidance in the notebook.

3. Upload the GFF3 format annotation file to Google colab temporal storage and execute this program.

***

# このプログラムについて
このプログラムは、GFF3ファイルから "source"、"type"、および"attributes"のキー文字列を抽出して出力するものです。    
GFF3フォーマットの詳細については、[GMOD wiki](http://gmod.org/wiki/GFF3)を参照してください。   

## インストール方法
インストール方法はリポジトリのトップページを参照してください。

## 使用法  
_I. 自分のコンピュータ上で実行する場合_
1. [Ensembl](http://m.ensembl.org/info/data/ftp/index.html)などから、GFF3形式のアノテーション情報ファイルをダウンロードしてください。  
線虫研究者の場合は、[wormbase ftp site](ftp://ftp.wormbase.org/pub/wormbase/species/) からダウロードできます。  

2. 
- Mac/Linux/Unixユーザ  
「ターミナル」アプリを起動してください。  
- Windows  
「コマンドプロンプト」アプリを起動してください。    

3. インストールしたソフトウェアのあるディレクトリに移動します。

4. コマンドを入力して実行します。

### 例
> $ ./GFFParser {SOMEWHERE}/Homo_sapiens.GRCh38.99.chr.gff3    


_II. Google Colab.上で実行する場合_
1. [テンプレートノートブック](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) をGoogle colab.に読み込みます。  

2. ノートブック内のガイダンスに従って、必要なライブラリとアプリケーション本体をインストールしてください。

3. 変換したいGFF3ファイルを入手して、Google colab.上のストレージに保存した後、プログラムを実行します。
