# AFIT-CSCE489-PROJ2
##### By: Capt Mike Pieschl
##### Date: 08 August 2024

---

#### Table of Contents

**1. Installation**

**2. Usage**

---

**1. Installation**

Linux installation steps are shown below. Please note that all CSCE489 projects will be cloned into the CSCE489 folder.

    git clone https://github.com/MAPieschl/CSCE489.git

    cd CSCE489_Project2

    make

**2. Usage**

To execute:

    ./babyyoda <buffer_size> <num_customers> <max_items> optional: <timing_slider>
    
Arguments:

	<buffer_size> - Represents the maximum number of items that can be stocked on the shelf at once.
	<num_customers> - The number of customers that will buy a baby yoda doll.
	<max_items> - The maximum number of items that will be produced.
				  NOTE:  If <max_items> exceeds <num_customers>, <max_items> will be reduced to match the value of <num_customers>. If this occurs, it will be annunciated at the start of execution.
	<timing_slider> - (optional) By default, the producer will produce an item at a pseudorandom interval between 1 and 5 seconds and a consumer will arrive at the store at a pseudorandom interval between 1 and 5 seconds. These values can be modified to:
		- Decrease production:  input a value between 1 and 9
		- Increase production: input a value between 11 and 19


