[![progress-banner](https://backend.codecrafters.io/progress/git/a9f76e27-2632-44ec-abce-418aace2f3be)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

This is a starting point for C++ solutions to the
["Build Your Own Git" Challenge](https://codecrafters.io/challenges/git).

In this challenge, you'll build a small Git implementation that's capable of
initializing a repository, creating commits and cloning a public repository.
Along the way we'll learn about the `.git` directory, Git objects (blobs,
commits, trees etc.), Git's transfer protocols and more.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

# Passing the first stage

The entry point for your Git implementation is in `src/Server.cpp`. Study and
uncomment the relevant code, and push your changes to pass the first stage:

```sh
git commit -am "pass 1st stage" # any msg
git push origin master
```

That's all!

# Stage 2 & beyond

Note: This section is for stages 2 and beyond.

1. Ensure you have `cmake` installed locally
1. Run `./your_program.sh` to run your Git implementation, which is implemented
   in `src/Server.cpp`.
1. Commit your changes and run `git push origin master` to submit your solution
   to CodeCrafters. Test output will be streamed to your terminal.

# Testing locally

The `your_program.sh` script is expected to operate on the `.git` folder inside
the current working directory. If you're running this inside the root of this
repository, you might end up accidentally damaging your repository's `.git`
folder.

We suggest executing `your_program.sh` in a different folder when testing
locally. For example:

```sh
mkdir -p /tmp/testing && cd /tmp/testing
/path/to/your/repo/your_program.sh init
```

To make this easier to type out, you could add a
[shell alias](https://shapeshed.com/unix-alias/):

```sh
alias mygit=/path/to/your/repo/your_program.sh

mkdir -p /tmp/testing && cd /tmp/testing
mygit init
```

## Installing packages
```bash
# add packages in vcpkg.json

vcpgk install
```

## Pushing to remote
```bash
git push -u origin main
```

# In testing folder
## Hashing keys and content
```bash
# list all files
find . -type f

# write a hash
git hash-object -w test.txt

# cat content
git cat-file -p 95d09f2b10159347eece71399a7e2e907ea3df4f
```
## Trees
```bash
# --add: use when the file doesn't yet exist in the index 
# --cacheinfo <mode> <object> <path>: use when file isn't in dir but in database e.g. the hash is old version of file
# 100644: file mode (regular file)
git update-index --add --cacheinfo 100644 95d09f2b10159347eece71399a7e2e907ea3df4f test.txt

# write the staging area (index) out to a tree object
git write-tree
git cat-file -p f03546f10f086a5cbc7b8580632ca6db2ba9411d # check tree's content
git cat-file -t f03546f10f086a5cbc7b8580632ca6db2ba9411d # check obj's type (tree)

echo 'new file' > new.txt
git update-index --add new.txt
git write-tree # after adding new file, tree gets new hash
git cat-file -p b8184f1bdf06d089b9b04c0c9a7eb8a8669a80bb

# read the first tree into staging area, as dir "bak" under current tree
git read-tree --prefix=bak f03546f10f086a5cbc7b8580632ca6db2ba9411d
git write-tree
git cat-file -p ab265692497c92dd980963dd5bbfeb11d54189d9
git ls-tree ab265692497c92dd980963dd5bbfeb11d54189d9 # same as above


# writing a tree
git init
git add .
git write-tree
git ls-tree 0b6e66

openssl zlib -d -in .git/objects/e1/79888d0f5f71d7c7c9bdd6c9d7a286f2de7772

tree <size>\0
40000 test_dir_1\0<sha>
40000 test_dir_2\0<sha>
100644 test_file_3.txt\0<sha>

5 + 1 + 10 + 1 + 20
5 + 1 + 10 + 1 + 20
6 + 1 + 15 + 1 + 20


tree 192\0
40000 octopus-admin\0 a84943494657751ce187be401d6bf59ef7a2583c
40000 octopus-deployment\0 14f589a30cf4bd0ce2d7103aa7186abe0167427f
40000 octopus-product\0 ec559319a263bc7b476e5f01dd2578f255d734fd
100644 pom.xml\0 97e5b6b292d248869780d7b0c65834bfb645e32a
40000 src\0 6e63db37acba41266493ba8fb68c76f83f1bc9dd

5 + 1 + 13 + 1 + 20
5 + 1 + 18 + 1 + 20
5 + 1 + 15 + 1 + 20
6 + 1 + 7 + 1 + 20
5 + 1 + 3 + 1 + 20
192
```

## Commiting
```bash
echo 'First commit' | git commit-tree 1d6753fb1a4263946e82a7ce64b7dcaa3191dfb2      
# 3edf745f1d5d7d50ec8c70773b96755166e988c3

git cat-file -p 1d6753fb1a4263946e82a7ce64b7dcaa3191dfb2
# 040000 tree afe59fc8f3bee47e8b018d0072a688dff7f2b8b5    test_dir_1
# 040000 tree 56280867b54d00c9c84b5440008ce95d464ed548    test_dir_2
# 100644 blob 95d09f2b10159347eece71399a7e2e907ea3df4f    test_file_1.txt
```