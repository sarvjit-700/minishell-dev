#!/bin/bash
# setup_tests.sh

echo "Setting up test files..."

# Vytvoření složek
mkdir -p test_files
mkdir -p outfiles

# Vytvoření souborů pro čtení
echo "Hello World" > test_files/infile
echo "Content of infile_big" > test_files/infile_big
# Přidáme víc řádků do big file
for i in {1..50}; do echo "line $i" >> test_files/infile_big; done

# Vytvoření souboru s mezerami
echo "Spaced file content" > "test_files/file name with spaces"

# Vytvoření souboru bez práv (pro testování permission denied)
touch test_files/invalid_permission
chmod 000 test_files/invalid_permission

# Simulace minishell.h (pokud ho nemáš)
touch minishell.h

# Úklid starých výstupů
rm -f outfiles/*
rm -f missing.out

echo "Setup done! Ready to test."
