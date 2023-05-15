# OS II Class Notes


# Exploration 6: RAID

If you were going to implement RAID at home, which level would you use (as a matter of your opinion), and why?

I think personally I would implement RAID 0 or 5. I have a comprehensive system for backing up my data, because of this RAID 1 wouldn't make sense for me and would reduce my storage capacity. 

Describe the small-write problem from the reading.

In the reading, the small-write problem occurs during RAID 4 when multiple operations are submitted to the RAID 4 system at the same time. The problem occurs when two small-write requests occur simultaneously causing a bottleneck when both write requests attempt to access the parity blocks on the parity disk. The parity disk is preventing parallelism and causing the writes to be serialized. 
