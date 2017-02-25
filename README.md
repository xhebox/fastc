# fastc
(WIP)yet another variety of fastq, to reduce the size of gene sequences greatly.

There're lots of AT(U)CG in gene sequences rather than letters that represent ambiguity, though IUPAC defined them. So why not encode four letters to one bit if people almost have no use of the other letters?



## Support format
At present, we plan to re encode the following format documents to save memory, as well as to provide a new program for subsequent data analysis  


* fasta
```
>gi|129295|sp|P01013|OVAX_CHICK GENE X PROTEIN (OVALBUMIN-RELATED)
GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCA
ACTCACAGTTTGATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAAT
CCATTTGTTCAACTCACAGTTT
```

* fastq
```
@SEQ_ID
GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACTCACAGTTT
+
!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65
```

*Notice:We only encode a sequence of AT (U) CG*
