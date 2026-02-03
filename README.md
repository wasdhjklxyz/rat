```bash
nix develop
mkdisks              # downloads rocky, creates disks
mkdocker             # pulls and creates rocky image
mkseed target        # cloud-init for target
mkseed operator      # cloud-init for operator
net up               # bridge up, airgapped

target &             # boots to login in ~30s
operator &           # same

build                # build binary
deploy               # move binary to vm

ssh rat@10.10.10.10  # target
ssh rat@10.10.10.20  # operator

net escape           # need to download stuff
net jail             # lock it back down
net down             # cleanup
```
