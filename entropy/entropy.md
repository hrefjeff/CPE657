To compile entropy.c: `gcc entropy.c -lm`

Usage: `./a.out <file>`

Output: `<measured entropy> <file>`

The measured entropy is on a scale between 0 and 8, with 0 being very low entropy (like repeated text) and 8 being very high entropy (like compressed or encrypted data).

To generate an encryption key:

1. `gpg --full-generate-key`
2. enter 1 for RSA & RSA encryption
3. enter desired keysize (4096 for a larger key)
4. enter 0 for no expiration date
5. provide name and email for key ID
6. set passphrase for private key (passphrase is required for decryption)

To encrypt a file using the key:

1. `gpg --encrypt --recipient <name from key ID> <filename>`
	- ex: `gpg --encrypt --recipient amber entropy.md`
