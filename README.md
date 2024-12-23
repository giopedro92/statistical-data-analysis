# Commands to run the code

---

---

>Author: Giovanni Pedrelli


## Run ROOT CERN docker

Run ROOT CERN docker
```bash
sudo docker run \
-e DISPLAY=$DISPLAY \
-v /tmp/.X11-unix:/tmp/.X11-unix \
--rm \
-it \
-v /home/giovanni-pedrelli/statistical-data-analysis/:/wsl/ \
--user $(id -u) \
-w /wsl/ \
rootproject/root bash
```

move to the correct directory
```bash
cd /wsl
```

move to the directory you want
```bash
cd ...
```

execute the file with root
```bash
root filename.C
```