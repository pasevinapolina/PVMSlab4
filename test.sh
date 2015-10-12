sudo rm -f /dev/first
sudo rm -f /dev/second
sudo rm -f /dev/operand
sudo rm -f /dev/result
sudo rmmod calc
make
sudo insmod ./calc.ko
modinfo calc.ko

sudo mknod /dev/first c 200 0
sudo mknod /dev/second c 200 1
sudo mknod /dev/operand c 200 2
sudo mknod /dev/result c 200 3

sudo chmod a+r+w /dev/first
sudo chmod a+r+w /dev/second
sudo chmod a+r+w /dev/operand
sudo chmod a+r+w /dev/result

echo "\n"
echo "10 + 2 = "
echo "10" > /dev/first
echo "2" > /dev/second
echo "+" > /dev/operand
cat /dev/result

echo "\n"
echo "1 - 7 = "
echo "1" > /dev/first
echo "7" > /dev/second
echo "-" > /dev/operand
cat /dev/result

echo "\n"
echo "5 * 2 = "
echo "5" > /dev/first
echo "2" > /dev/second
echo "*" > /dev/operand
cat /dev/result

echo "\n"
echo "30 / 3 = "
echo "30" > /dev/first
echo "3" > /dev/second
echo "/" > /dev/operand
cat /dev/result

echo "\n"
echo "10 / 0 = "
echo "10" > /dev/first
echo "0" > /dev/second
echo "/" > /dev/operand
cat /dev/result

echo "\n"
echo "10 e 2 ="
echo "10" > /dev/first
echo "2" > /dev/second
echo "e" > /dev/operand
cat /dev/result

sudo rm -f /dev/first
sudo rm -f /dev/second
sudo rm -f /dev/operand
sudo rm -f /dev/result
sudo rmmod calc
make clean
