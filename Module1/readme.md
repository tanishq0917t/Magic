### Instructions
##### How to compile source files using "tanishq" library
```
gcc filename.c -I ../tanishq/include/ -L ../tanishq/lib/ -ltanishq -o filename -Wall
```
```-I``` -> where is header files?\
```-L``` -> where is library file?\
```-l``` -> library name? (without lib prefix)\
```-Wall``` -> To show all warnings as error during compilation
### Using Tool to generate Training and Testing Data 
```
Usage: [create_test_train_dataset dataset test_file train_file minor_percentage]
```
<ul>
  <li><b>create_test_train_dataset</b> - Executable to generate test & train data</li>
  <li><b>dataset</b> - Raw Dataset</li>
  <li><b>test_file</b> - Testing Dataset File name to be created</li>
  <li><b>train_file</b> - Training Dataset File name to be created</li>
  <li><b>minor_percentage</b> - Minor Percentage (% record to be stored in Testing Dataset)</li>
</ul>
