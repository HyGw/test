/* small demo app that just includes a cipher/hash/prng */
#include <tomcrypt.h>

int main(void)
{
   register_cipher(&rijndael_enc_desc);
   register_prng(&yarrow_desc);
   register_hash(&sha256_desc);
   return 0;
}

/* $Source: /cvs/libtom/libtomcrypt/demos/small.c,v $ */
/* $Revision: #1 $ */
/* $Date: 2016/10/14 $ */
