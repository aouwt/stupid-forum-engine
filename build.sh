g++ ./src/*.cpp -Wall -Wextra -Wpedantic -lcgi -lsqlite3 -lrandom -largon2 -o ./fourm.cgi -O0 && 
sudo cp ./fourm.cgi /lib/cgi-bin/fourm.cgi
sudo mkdir /var/www/html/fourm
sudo cp -r ./static/* /var/www/html/fourm/
