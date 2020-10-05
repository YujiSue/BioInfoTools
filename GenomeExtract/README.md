[English](#About)/[日本語](#このプログラムについて)

## About
This program extracts the sequence of target region from binary genomic data encoded by GenomeConvert.  

## Install
Please see [Installation of BioInfoTools]().

## Usage
_I. For users who execute the program on your computer._
1. Launch the CUI software. 
- Mac/Linux/Unix  
Launch the "Terminal" application.  
- Windows  
Launch the "Command Prompt".  

2. Move to the directory, in which the GenomeExtract was installed.

3. Exec the command.
```  
Usage:
  GenomeExtract -r file position(s) { --rev } { -f txt/fa }
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[Option]
  --format/-f         :  File format to display or export.
  --ref/-r            : *Reference file path.
  --rev               :  Get reverse strand seq.

  --help/-H           : Show help.
  --info/-I           : Show app information.
  --version/-V        : Show version.

  * Required option.
  # Choose one of the options.
```

### Example
> ./GenomeExtract -r {SOMEWHERE}/human.bin --rev 17:7687000..7687550

```  
Result:  
GATGGGATTGGGGTTTTCCCCTCCCATGTGCTCAAGACTGGCGCTAAAAGTTTTGAGCTTCTCAAAAGTCTAGAGCCACCGTCCAGGGAGCAGGTAGCTGCTGGGCTCCGGGGACACTTTGCGTTCGGGCTGGGAGCGTGCTTTCCACGACGGTGACACGCTTCCCTGGATTGGGTAAGCTCCTGACTGAACTTGATGAGTCCTCTCTGAGTCACGGGCTCTCGGCTCCGTGTATTTTCAGCTCGGGAAAATCGCTGGGGCTGGGGGTGGGGCAGAGGGGACTTAGCGAGTTTGGGGGTGAGTGGGATGGAAGCTTGGCTAGAGGGATCATCATAGGAGTTGCATTGTTGGGAGACCTGGGTGTAGATGATGGGGATGTTAGGACCATCCGAACTCAAAGTTGAACGCCTAGGCAGAGGAGTGGAGCTTTGGGGAACCTTGAGCCGGCCTAAAGCGTACTTCTTTGCACATCCACCCGGTGCTGGGCGTAGGGAATCCCTGAAATAAAAGATGCACAAAGCATTGAGGTCTGAGACTTTTGGATCTCGAAA
```  

_II. For users who execute the program on google colab._
1. Open the [template notebook](https://github.com/YujiSue/BioInfoTools/blob/master/BioInfoTools_Template.ipynb) on your Google colab.  

2. Install the required libraries and GenomeExtract according to the guidance in the notebook.

3. Upload the GFF3 format annotation file to Google colab temporal storage and execute this program.

***

## このプログラムについて