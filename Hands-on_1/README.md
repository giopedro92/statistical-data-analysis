# Commands to run the code

---

---

>Author: Giovanni Pedrelli


## Run ROOT CERN docker
```bash
sudo docker run \
-e DISPLAY=$DISPLAY \
-v /tmp/.X11-unix:/tmp/.X11-unix \
--rm \
-it \
-v ~:/wsl \
--user $(id -u) \
rootproject/root root
```