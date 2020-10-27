#!/bin/bash
set -ev

THREAD=1
SEND=1

while getopts t:s:d: option
do
case "${option}"
in
t) THREAD=${OPTARG};;
s) SEND=${OPTARG};;
d) DOCS_ONLY=${OPTARG};;
esac
done


mkdir build_cov
export GCOV_PREFIX=`pwd`/build_cov/data
cd build_cov

if [[ $DOCS_ONLY == 0 ]] ;
then

export GCOV_PREFIX_STRIP=`pwd | awk -F"/" '{print NF-1}'`
echo $GCOV_PREFIX
echo $GCOV_PREFIX_STRIP

cmake .. -DCOVERAGE_BUILD=ON -DCMAKE_BUILD_TYPE=Debug

make -j$THREAD complete_test_suite

./complete_test_suite

cd CMakeFiles
cp --parents `find . -name '*.gcno'` ../data
cd ..
export GCOV_PREFIX=`pwd`
export GCOV_PREFIX_STRIP=`pwd | awk -F"/" '{print NF-1}'`
echo $GCOV_PREFIX
echo $GCOV_PREFIX_STRIP
make fesutils_coverage
if [[ $SEND != 0 ]] ;
then
bash <(curl -s https://codecov.io/bash) -f fesutils_coverage.info || echo "Codecov did not collect coverage reports";
fi


fi

echo "Documentation coverage"
pip3 install coverxygen
cd ..
doxygen Doxyfile
cd build_cov
python3.8 -m coverxygen --xml-dir ../docs/doxygen/xml --src-dir ../src --output doc-coverage.info  --scope all  --kind all --format summary
#
#
# =============================================
# ======= DOCUMENTATION COVERAGE ! ============
# =============================================
#
cat doc-coverage.info
