# Table of Contents

- [About the project](#macutils)
	- [Build and install](#build-and-install)
	- [Usage](#usage)
        - [maclookup](#maclookup)
        - [macrandom](#macrandom)
        - [update of the database](#update-of-the-database)
	- [Input](#input)
- [How does it work](#how-does-it-work)
- [License](#license)
- [Contact](#contact)

# macutils

macutils is a suite of tools designed to manipulate mac addresses.
It is made of 2 tools:
- maclookup
- macrandom

## maclookup

maclookup queries the [iee's oui.txt database to](http://standards-oui.ieee.org/oui/oui.txt) find the manufacturer of a network interface.

## macrandom

macrandom generates a valid mac address (with a valid manufacturer) using the iee's oui.txt database.    
This is unlike most internet based tools, which only generate a fully random mac address, which very often corresponds to no real manufacturer.

# Build and install

Install dependencies (fedora):
```
sudo dnf install libcurl-devel curl
```

Build the project:
```
make
```

Install:
```
sudo make install
```

By default, maclookup is installed in `/usr/bin/maclookup`  and macrandom in installed in `/usr/bin/macrandom`.
During installation, the latest oui.txt will also be downloaded from the iee website : http://standards-oui.ieee.org/oui/oui.txt and installed in `/usr/local/share/maclookup`.

Uninstall:
```
sudo make uninstall
```

A manual is also installed for each tool in both english and french in `/usr/local/share/man`. It can be consulted with the following commands:
```
man maclookup
```
and
```
man macrandom
```

# Usage

Since maclookup creates a local copy of the oui.txt database on the host, all macutils tools can be used offline.
It is however necessary to have an internet connection to update the database.

## maclookup

maclookup is designed for a quick usage in a terminal:

The classical usage is the following:
```
$ maclookup 7C-F9-A0-66-EE-94
7C-F9-A0   (hex)                Fiberhome Telecommunication Technologies Co.,LTD
7CF9A0     (base 16)            Fiberhome Telecommunication Technologies Co.,LTD
                                No.5 DongXin Road
                                Wuhan  Hubei  430074
                                CN

```

It can also be used in a data pipe:
```
$  echo "7C-F9-A0-66-EE-94" | maclookup -s
7C-F9-A0   (hex)                Fiberhome Telecommunication Technologies Co.,LTD
7CF9A0     (base 16)            Fiberhome Telecommunication Technologies Co.,LTD
                                No.5 DongXin Road
                                Wuhan  Hubei  430074
                                CN

```
To obtain help, you can type:
```
maclookup --help
```

## macrandom

The classical usage is the following:
```
$ macrandom 
-------------------------------------------------

Randomly generated mac address: C8-8D-83-AC-0B-39

-------------------------------------------------

Manufacturer informations:

C8-8D-83   (hex)                HUAWEI TECHNOLOGIES CO.,LTD
C88D83     (base 16)            HUAWEI TECHNOLOGIES CO.,LTD
                                No.2 Xin Cheng Road, Room R6,Songshan Lake Technology Park
                                Dongguan    523808
                                CN

```
To obtain help, you can type:
```
macrandom --help
```

## Update of the database

We can update the oui.txt database using the following command:
```
sudo maclookup -u
```

A backup of the database is created during update. If the update is interrupted, the backup will be restored.

# Input

macrandom takes no input.

The input of maclookup is a valid full length mac address or the first 8 characters (manufacturer prefix).

# How does it work

macutils tools use the iee's oui.txt database.
It identifies all the manufacturers of network interfaces, with their 6 hexadecimal digits prefix.

Both macrandom and maclookup read this database to find a manufacturer prefix.

The database is downloaded during install using curl.
Later updates are done using libcurl.

# License

Distributed under GPL License. See `COPYING` for more information.

# Contact

Mathias Schmitt - mathiaspeterhorst@gmail.com     
Project link: https://github.com/mphschmitt/chktropy
