# PyCodec

This directory contain all stuff related to PyCodec.

PyCodec use different encoding system on different OS. In Windows it use Win32 functions, and it will use Iconv in other OS. So we need a table converting PyCodec universal encoding name to Windows Code Page or Iconv Code Name. These relation was stored in CSV file and Python script will render it into C++ source code.

For the format of CSV file, each line is a record. The first item in record is the standard PyCodec name. The second item is corresponding Windows Code Page. If there is no corresponding Code Page, it can be empty. The third item is corresponding Iconv Code Name. It also can be empty with same case. Then, the count of remain columns is variables after forth item (inclusive). All of them is the alias of this standard PyCodec name.
