VER=$(git describe --long | sed s/^v// | sed s/-g.*// | sed s/-/\./)
if [ -f "version_info" ]; then
	rm version_info
fi

if [ "$VER" == "" ]; then
	VER=0.0.0.0
fi
echo VERSION=$VER > version_info
