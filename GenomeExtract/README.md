

============================================================
GenomeExtract v1.1.0
Copyright (c) 2015 Yuji Suehiro. All rights reserved.
MIT license
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
============================================================

### Example
> ./GenomeExtract -r {SOMEWHERE}/human.bin --rev 17:7687000..7687550

```  
Result:  
GATGGGATTGGGGTTTTCCCCTCCCATGTGCTCAAGACTGGCGCTAAAAGTTTTGAGCTTCTCAAAAGTCTAGAGCCACCGTCCAGGGAGCAGGTAGCTGCTGGGCTCCGGGGACACTTTGCGTTCGGGCTGGGAGCGTGCTTTCCACGACGGTGACACGCTTCCCTGGATTGGGTAAGCTCCTGACTGAACTTGATGAGTCCTCTCTGAGTCACGGGCTCTCGGCTCCGTGTATTTTCAGCTCGGGAAAATCGCTGGGGCTGGGGGTGGGGCAGAGGGGACTTAGCGAGTTTGGGGGTGAGTGGGATGGAAGCTTGGCTAGAGGGATCATCATAGGAGTTGCATTGTTGGGAGACCTGGGTGTAGATGATGGGGATGTTAGGACCATCCGAACTCAAAGTTGAACGCCTAGGCAGAGGAGTGGAGCTTTGGGGAACCTTGAGCCGGCCTAAAGCGTACTTCTTTGCACATCCACCCGGTGCTGGGCGTAGGGAATCCCTGAAATAAAAGATGCACAAAGCATTGAGGTCTGAGACTTTTGGATCTCGAAA
```  