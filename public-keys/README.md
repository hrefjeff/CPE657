# Commands to generate public-private key pair

`ssh-keygen`

Create keypair without a password

Let the tool generate the keypair in the ~/.ssh folder

`cp ~/.ssh/id-rsa.pub /path/to/CPE657/public-keys/YOURNAME.pub`

# 2. Upload key to gitlab

Navigate to https://gitlab.com/-/profile/keys and upload your id_rsa.pub

Navigate to https://github.com/settings/keys and upload your id_rsa.pub
