# Move this to the Makefile in the directory, where nussinov is implemented

test-nussinov: nussinov.py
	./nussinov.py phenyalanines.txt | diff - fold-phenyalanines.xml

# Move this to the Makefile in the directory, where rna_count is implemented

test-numstructs:
	rna_count 16 | head -n 16 | diff - numstructs.tsv


