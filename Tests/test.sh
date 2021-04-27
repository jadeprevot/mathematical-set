echo "-----------------------------------------------------------"

execDir=`pwd`

if [ "$1" = "" ]
then
  echo "No directory given, default to current"
  Directory="."
else  
  echo "| Test id : $1"
  if [ -d "$1" ]
  then 
     Directory="$1"
  else
     echo "$1 is not a directory. Exiting."
     exit 2
  fi
fi

cd $Directory

if [ -r "description" ]
then 
  echo "-----------------------------------------------------------"
  echo "Description :"
  fold description -w 60 -s
  echo "-----------------------------------------------------------"
fi

nTestCount=0
nSuccesfulTests=0
nStrResult="$1 "

if [ -r "run" ]
then
  sRun=`cat run`
else
  echo "No run file found. Exiting."
  exit 2
fi


# stdout has been specified
if [ -r "std.out" ]
then 
  sRun="$sRun >temp.txt"
fi

echo $sRun
# execute the command line
eval $sRun
returnCode=$?

resultGlobal=1

# compare stdout if concerned
resultOut=2
if [ -r "std.out" ]
then 
  diff -wB temp.txt std.out >/dev/null
  if [ $? -eq 0 ]
  then
    echo "                                       Stdout      : PASSED"
    resultOut=1
  else
    echo "                                       Stdout      : FAILED"
    resultOut=0
    resultGlobal=0
  diff -wB -y --side-by-side  temp.txt std.out 
  fi
  # clean temporary out file
  rm temp.txt
fi

echo "                                       --------------------"
if [ $resultGlobal -eq 0 ]
then
  echo "                                       Global      : FAILED"
else
  echo "                                       Global      : PASSED"
fi
echo "-----------------------------------------------------------"
echo 

cd $execDir

# log result in $2 if filename provided
if [ "$2" != "" ]
then
  if [ ! -w "$2" ]
  then
    touch $2
  fi
  if [ -w "$2" ]
  then
    echo "$Directory;$resultOut;$resultGlobal" >>$2
  fi
fi

exit $resultGlobal

