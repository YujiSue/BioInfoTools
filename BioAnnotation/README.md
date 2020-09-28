

### Usage
```
============================================================
BioAnnot v1.1.0
Copyright (c) 2015 Yuji Suehiro. All rights reserved.
MIT license.
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Usage:
  BioAnnot -d database -f source(pos/gene/trs/mut) -t output(pos/gene/trs/mut) input { -s } { -g id/name }
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

[Option]
  --db/-d             : *Annotation database file path.
  --from/-f           : *Genomic position([Chr]:[Start]..[End]), Gene name, Transcript name, or Mutant name.
  --gtype/-g          :  Type of gene output(Unique gene ID, or Gene name.
  --to/-t             : *Genomic position([Chr]:[Start]..[End]), Gene name, Transcript name, or Mutant name.
  --tsite/-s          :  Show sites of transcripts (CDS/exon/intron/5'UTR/3'UTR).

  --help/-H           : Show help.
  --info/-I           : Show app information.
  --version/-V        : Show version.

  * Required option.
  # Choose one of the options.
============================================================
```

### Example
> $ ./BioAnnotation -d {SOMEWHERE}/human.db -f gene -t pos TP53  
```  
Result:  
TP53    >>      17:7661779..7687550(-)
```  

