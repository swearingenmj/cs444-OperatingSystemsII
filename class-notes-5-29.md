# OS II Class Notes


# Exploration 9

Do the math to determine if this is a valid credit card number (the last digit ```5``` is the checksum!):

```4485 0803 6191 2535```

(For the record, even if it is valid, it's not real!)

4 4 8 5  0 8 0 3  6 1 9 1  2 5 3 
1 2 1 2  1 2 1 2  1 2 1 2  1 2 1   <--- double every other digit starting with the rightmost digit
=======  =======  =======  ====== 
4 8 8 10 0 16 0 6 6 2 9 2  2 10 3  <--- sum the digits of the doubled numbers
4 8 8 1  0 7  0 6 6 2 9 2  2 1  3

The sum of the resulting digits is 59.
The check digit is equal to 10 - (59 mod 10) = 10 - 9 = 1.

4485 0803 6191 2531 != 4485 0803 6191 2535 (invalid)

Why is adding together all the bytes and then modding by 256 not a good checksum? What errors could be introduced that the checksum wouldn't catch?

This is not a good checksum because it is limited in the types of errors it can detect. For example, if two bytes are swapped the same checksum would be generated leading to an undetected error. Additionally, the checksum can't detect other forms of transposition because the sum would be the same.  
