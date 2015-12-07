temp="temp.html"
url=$1
path=$2
# echo This is the path: $path
# echo This is the url: $url
cd $path
wget -q -O $temp $url
if [ $? -ne 0 ]; then
    echo "Could not access webpage. Exitting program"
    exit 1
fi

exit 0