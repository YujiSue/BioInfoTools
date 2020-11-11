# AnnotDBMaker

[English](#About)/[日本語](#このプログラムについて)

## About
This is a core program to generate sqlite3 database for gene annotation from GFF3 format file and other additional files.  
To run this program, you will need plug-ins tailored to the annotation data of each species.

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

3. Move to the directory, in which the AnnotDBMaker was installed.

4. Copy the plug-in to the same directory as AnnotDBMaker.

4. Exec the command.

### Example1 (human)
> $ ./AnnotDBMake -r {SOMEWHERE}/human.bin -p human.so/.dll -g {SOMEWHERE}/Homo_sapiens.GRCh38.99.chr.gff3 -d GRCh38.99 -s "Homo sapiens" -c "Yuji Suehiro" -o {SOMEWHERE}/human.db 

### Example2 (nematode)
> $ ./AnnotDBMake -r {SOMEWHERE}/nematode.bin -p nematode.dll -g {SOMEWHERE}/c_elegans.PRJNA13758.WS274.annotations.gff3 -d WS274 -s "Caenorhabditis elegan" -c "Yuji Suehiro" -e "gene:{SOMEWHERE}/c_elegans.PRJNA13758.WS274.geneIDs.txt,description:{SOMEWHERE}/c_elegans.PRJNA13758.WS274.functional_descriptions.txt" -o {SOMEWHERE}/nematode.db


_II. For users who execute the program on google colab._
1. Open the [template notebook](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) on your Google colab.  

2. Install the required libraries and GenomeConverter according to the guidance in the notebook.

3. Upload the FASTA format genomic sequence to Google colab. temporal storage and execute this program.

## File format
The encoded binary DNA sequence is 

***

## このプログラムについて
このプログラムは、GFF3形式のファイルをもとにアノテーション用のsqlite3データベースを作成するコアプログラムです。  
プログラムを使用するには、別途プラグイン（共有/ダイナミックライブラリ）が必要です。  
現在、ヒトと線虫用のプラグインが準備されています。  

## インストール方法
インストール方法はリポジトリのトップページを参照してください。

## 使用法  
_I. 自分のコンピュータ上で実行する場合_
1. [Ensembl](http://m.ensembl.org/info/data/ftp/index.html)などから、GFF3ファイルをダウンロードしてください。  
線虫研究者の場合は、[wormbase ftp site](ftp://ftp.wormbase.org/pub/wormbase/species/) からダウロードできます。  

2. 
- Mac/Linux/Unixユーザ  
「ターミナル」アプリを起動してください。  
- Windows  
「コマンドプロンプト」アプリを起動してください。    

3. インストールしたソフトウェアのあるディレクトリに移動します。

4. コマンドを実行します。

### 例１ (ヒト)  
> $ ./AnnotDBMake -r {SOMEWHERE}/human.bin -p human.so/.dll -g {SOMEWHERE}/Homo_sapiens.GRCh38.99.chr.gff3 -d GRCh38.99 -s "Homo sapiens" -c "Yuji Suehiro" -o {SOMEWHERE}/human.db 

### 例２ (線虫)  
> $ ./AnnotDBMake -r {SOMEWHERE}/nematode.bin -p nematode.dll -g {SOMEWHERE}/c_elegans.PRJNA13758.WS274.annotations.gff3 -d WS274 -s "Caenorhabditis elegan" -c "Yuji Suehiro" -e "gene:{SOMEWHERE}/c_elegans.PRJNA13758.WS274.geneIDs.txt,description:{SOMEWHERE}/c_elegans.PRJNA13758.WS274.functional_descriptions.txt" -o {SOMEWHERE}/nematode.db

_II. Google Colab.上で実行する場合_
1. [テンプレートノートブック](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) をGoogle colab.に読み込みます。  

2. ノートブック内のガイダンスに従って、必要なライブラリとアプリケーション本体をインストールしてください。

3. 変換したいFASTAシーケンスファイルを入手して、Google colab.上のストレージに保存した後、プログラムを実行します。


