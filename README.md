[English](##About)/[日本語](##このリポジトリについて)

# BioInfoTools
## About
This repository contains some programs using for BioInformatics analysis on my research.

## Installation of BioInfoTools
1. Install the required libraries.  
1-1. Build by yourself  
1-1-1. Install Python3.X  
You can download an install package from [Python.org](https://www.python.org/downloads/).  
Download the package and install it according to the official guidance.  
1-2. If you copy pre-build libraries, you need to download the following 3 libraries from [my site]().
* libsobj  
* libsapp
* libsbioinfo


### For Windows users
Execute the command on the command prompt.
```
> 
```

## Usage
The details how to use are described on README.md in each program directory.  
Please refer to the document.

***

## このプログラムについて
このリポジトリは、主に研究で使用しているバイオインフォマティクス関連のプログラム群を集めたものです。

## インストール方法
### I. 自分のPC上にインストールする場合
#### 1. 必要なライブラリをインストールしてください。  
このプログラムは[slib](https://github.com/YujiSue/slib)内の以下のライブラリが必要です。  
* libsobj  
* libsapp  
* libsbioinfo  

  1-1. 自分でビルドする場合...  
    1-1-1. Python3.x をインストールしてください。  
    [Python.org]()からインストール用パッケージをダウンロードし、ガイダンスに従って、インストールしてください。  
    1-1-2. ビルド用のツールをインストールしてください。  
    ※Unix/Linuxを使用している人は必要ありません。  
    Macユーザは[XCode]()をインストールしてください。  
    Windowsユーザは[Visual Studio 2019]()をインストールしてください。
    1-1-3. [slibのコード]()をダウンロードして、解凍してください。  
    1-1-4. Unix/Linux/Macユーザはターミナルを、Windowsユーザはコマンドプロンプトを起動して、以下のコマンドを順に実行してください。
    ```
    $ cd {ダウンロード先}/slib-master  
    $ ./gyp configure
    $ ./gyp build
    ```  
    1-1-5. slib-masterフォルダ内にbuildフォルダが新たに形成されます。  
  
  1-2. 専用インストーラを使う場合...  
  1-2-1. [私のサイト]()から、自分の使用しているOSに合わせて、インストーラをダウンロードしてください。  
  1-2-2. インストーラを起動したら、

  


## 使用法
各プログラムの使用法の詳細は、各々のプログラムのディレクトリ内にあるREADME.mdファイルを参照してください。
