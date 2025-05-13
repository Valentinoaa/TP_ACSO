# TP-3 File System Unix v6 

#### Antes de comenzar lean bien el enunciado e intenten comprender que hace cada una de las partes del TP.

- El proyecto se puede construir ejecutando el siguiente comando:
  
      make
- Este generará un ejecutable diskimageaccess que se puede probar de la siguiente manera:

      ./diskimageaccess ​<options>​ ​<diskimagePath>

- Se puede comparar su salida contra la de la cátedra (les proveemos un ejecutable solucion en **/samples**):

      ./samples/diskimageaccess_soln ​<options>​ ​<diskimagePath>
  
  (En caso de que no lo puedan ejecutar les proveemos la salida de este para que la prueben contra su salida)

- <**diskimagePath**>: debe ser la ruta a uno de los discos de prueba ubicados en:

       ./samples/testdisks. 

en el direcetorio **sample/testdisks**, hay tres discos de prueba: basicDiskImage, depthFileDiskImage y dirFnameSizeDiskImage.

- El ejecutable diskimageaccess reconoce validas solo dos <**options**>:

      i: prueba las capas de inode y archivo.
      p: prueba las capas de nombre de archivo y ruta.

- Por ejemplo, para ejecutar ambas pruebas de inode y nombre de archivo en el disco basicDiskImage, se puede ejecutar:

      ./diskimageaccess -ip ./samples/testdisks/basicDiskImage
  
La salida esperada de ejecutar *diskimageaccess* en cada imagen de disco X se almacena en el archivo **X.gold** dentro del directorio **/samples/testdisks** y tiene que coincidar con su salida. Esto genera un output en la salida estándar, ustedes la pueden redirigir a un archivo haciendo lo siguiente:

     ./diskimageaccess -ip /samples/testdisks/basicDiskImage > output_basic.txt

En el caso de que no puedan puedan ejecutar **diskimageaccess_soln_x8**. Pueden comparar su salida (**output_basic.txt**) con la de la cátedra que son los archivos que tienen extensión **.gold** que se encuentran en **/samples/testdisks**. La idea es que la salida de ejecutar el comando:

    diff output_basic.txt basicDiskImage.gold

no encuentre ninguna diferencia entre esos archivos.
