#!/usr/bin/env sh

echo Building Precompiled Header...
cd src/ascent-world
g++ -pthread -Wall -DHAVE_CONFIG_H -I. -I../.. -I/usr/include/mysql -I/usr/include/openssl -I/usr/local/include/openssl -I/usr/local/include -I. -I./../ascent-shared -I./../ascent-shared/vmap -I./../ascent-shared/g3dlite -o StdAfx.h.gch StdAfx.h

echo Done. Now you can run make, and experience much faster build times. Remember to re-run this script every time you svn update.


