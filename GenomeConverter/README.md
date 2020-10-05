# GenomeConverter

[English](#About)/[日本語](#このプログラムについて)

## About
This program is a file converter from FASTA format DNA sequene to binary file, in which the DNA sequence was compressed to 1 byte / 4 bases.

## Install
Please see [Installation of BioInfoTools]().

## Usage
_I. For users who execute the program on your computer._
1. Download the FASTA format genomic sequence from database site such as [RefSeq](https://www.ncbi.nlm.nih.gov/refseq/).  
For nematode researchers, I think the [wormbase ftp site](ftp://ftp.wormbase.org/pub/wormbase/species/) is the best choice.

2. Launch the CUI software. 
- Mac/Linux/Unix  
Launch the "Terminal" application.  
- Windows  
Launch the "Command Prompt".  

3. Move to the directory, in which the GenomeConverter was installed.

4. Exec the command.
```  
Usage:
  GenomeConverter -o 'output' input(s) { -l } { -s name } { -r version }
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[Option]
  --linkage/-l        :  Select only linkaged sequence.
  --out/-o            : *Output file path.
  --refver/-r         :  Reference version.
  --species/-s        :  Species.

  --help/-H           : Show help.
  --info/-I           : Show app information.
  --version/-V        : Show version.

  * Required option.
  # Choose one of the options.
```  

### Example1 (human)  
>$ ./GenomeConverter -l -s "H. sapiens" -r GRCh38.p13 -o {SOMEWHERE}/human.bin {SOMEWHERE}/GCF_000001405.39_GRCh38.p13_genomic.fna  

### Example2 (nematode)  
>$ ./GenomeConverter -l -s "C. elegans" -r WS274 -o {SOMEWHERE}/nematode.bin {SOMEWHERE}/c_elegans.PRJNA13758.WS274.genomic.fa


_II. For users who execute the program on google colab._
1. Open the [template notebook](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) on your Google colab.  

2. Install the required libraries and GenomeConverter according to the guidance in the notebook.

3. Download the FASTA format genomic sequence to Google colab's temporal storage or google drive and execute the program.

## File format of encoded sequence
The encoded binary DNA sequence is 

***

## このプログラムについて
このプログラムは、FASTA形式のDNA塩基配列を４塩基１バイトに圧縮したバイナリファイルに変換するソフトです。

## インストール方法
インストール方法は、 [BioInfoToolsのインストール]()を参照してください。

## 使用法  
_I. 自分のコンピュータ上で実行する場合_
1. [RefSeq](https://www.ncbi.nlm.nih.gov/refseq/)などから、ゲノムのFASTAファイルをダウンロードしてください。  
2, 線虫研究者の場合は、[wormbase ftp site](ftp://ftp.wormbase.org/pub/wormbase/species/) からダウロードできるFASTAファイルの方が最新です。  

2. 
- Mac/Linux/Unixユーザ  
「ターミナル」アプリを起動してください。  
- Windows  
「コマンドプロンプト」アプリを起動してください。    

3. インストールしたソフトウェアのあるディレクトリに移動します。

4. コマンドを入力して実行します。
```  
Usage:
  GenomeConverter -o 'output' input(s) { -l } { -s name } { -r version }
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[Option]
  --linkage/-l        :  Select only linkaged sequence.
  --out/-o            : *Output file path.
  --refver/-r         :  Reference version.
  --species/-s        :  Species.

  --help/-H           : Show help.
  --info/-I           : Show app information.
  --version/-V        : Show version.

  * Required option.
  # Choose one of the options.

```  

### 例１ (ヒト)  
>$ ./GenomeConverter -l -s "H. sapiens" -r GRCh38.p13 -o {SOMEWHERE}/human.bin {SOMEWHERE}/GCF_000001405.39_GRCh38.p13_genomic.fna  

### 例２ (線虫)  
>$ ./GenomeConverter -l -s "C. elegans" -r WS274 -o {SOMEWHERE}/nematode.bin {SOMEWHERE}/c_elegans.PRJNA13758.WS274.genomic.fa

_II. Google Colab.上で実行する場合_
1. [テンプレートノートブック](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) をGoogle colab.に読み込みます。  

2. ノートブック内のガイダンスに従って、必要なライブラリとアプリケーション本体をインストールしてください。

3. 変換したいFASTAシーケンスファイルを入手して、Google colab.上のストレージに保存した後、プログラムを実行します。

## ファイル形式

