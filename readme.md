# AudioSwitch

## Switch to your headphone jack when you plug in your headphones

I developed this for my Xonar DG sound card, it probably doesn't work with other sound cards out of the box, and may not work with other Xonar DG's.

It's likely that any sound card that uses Oxygen HD Audio driver in linux will be able to use this if you modify the location of the headphone jack status and the location of the oxygen file in switcher.py. 

### Configuration and installation

```
./configure
make
sudo make install
```

Then it can be run with `audioswitch`

If you want to run it as a service you can copy `audioswitch.service` to `/etc/systemd/system` then run `sudo systemctl start audioswitch.service` to start it, and `sudo systemctl enable audioswitch.service` to start it automatically on boot.
