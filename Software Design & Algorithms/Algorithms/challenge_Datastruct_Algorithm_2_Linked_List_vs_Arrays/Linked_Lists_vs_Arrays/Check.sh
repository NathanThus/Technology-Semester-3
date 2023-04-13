clear

echo ""
echo "=================================  Making LinkedList  ================================="
echo "Running Command: make"
echo ""
make

echo ""
echo "===================  Running LinkedList with input: input_full.txt  ==================="
echo "Running Command: valgrind ./linkedlist < testdata/input_full.txt > output_full.txt"
echo ""
valgrind ./linkedlist < testdata/input_full.txt > output_full.txt

echo ""
echo "=================== Running LinkedList with input: input_simple.txt ==================="
echo "Running Command: valgrind ./linkedlist < testdata/input_simple.txt > output_simple.txt"
echo ""
valgrind ./linkedlist < testdata/input_simple.txt > output_simple.txt

echo ""
echo "===================  Comparing output_full.txt and correct_full.txt  =================="
echo "Running Command: diff --side-by-side --suppress-common-lines output_full.txt testdata/correct_full.txt"
echo ""
diff --side-by-side --suppress-common-lines output_full.txt testdata/correct_full.txt

echo ""
echo "=================  Comparing output_simple.txt and correct_simple.txt  ================"
echo "Running Command: diff --side-by-side --suppress-common-lines output_simple.txt testdata/correct_simple.txt"
echo ""
diff --side-by-side --suppress-common-lines output_simple.txt testdata/correct_simple.txt