### Relevant portions of the code.

##### `CSCValidation.cc`
This does the bulk of the work. In particular,
`CSCValidation::doSegments()` does the computation for variables going into the tree
of segments: `histos->fillSegmentTree()`. This latter function call must match the 
definitions in the following files.

##### `CSCValHists.cc`
`CSCValHists::fillSegmentTree()` is implemented here.  Additionally, the tree is filled with the monstrosity starting with `segTree->Branch(`;

##### `CSCValHists.h`
`struct posRecord` contains the information for a segment.


### Analyzing the output
You can start looking at the output with
```
root output.root
Segments->cd()
segPositions->Draw("chamber","endcap == 1 && ring == 2 && station == 4")
```
Note that the tree has one entry per segment. Other branches are found in the previous src files.
