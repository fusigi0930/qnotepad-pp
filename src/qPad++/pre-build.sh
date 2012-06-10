VER=$(git describe)
if [ "$VER" == "" ]; then
	VER=0.0.0.0
fi
echo VERSION=$VER > version_info