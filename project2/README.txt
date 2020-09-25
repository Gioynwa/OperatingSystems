Paraskevi Bakami 2117
Dimitrios Kladouchos 2189
Georgios Simos 2180

In schedule.c there is a define line : #define exp 0. 
Comment this line to run for minimun Goodness. 
Uncomment this line to run for minimum Expected Burst.

Experiment Results for last process deactivation time:

1) Interactive :

	Expected Burst VS Goodness: 1999ms VS 1999ms

	Expected Burst VS Goodness(Without waiting in rq time): 1999ms VS 1999ms

	Goodness VS Goodness(Without waiting in rq time): 1999ms VS 1999ms


2) Non-Interactive :

	Expected Burst VS Goodness: 1879ms VS 1879ms

	Expected Burst VS Goodness(Without waiting in rq time): 1879ms VS 1178ms

	Goodness VS Goodness(Without waiting in rq time): 1879ms VS 1178ms*


3) Both :

	Expected Burst VS Goodness: 1049ms VS 1050ms

	Expected Burst VS Goodness(Without waiting in rq time): 1049ms VS 1049ms

	Goodness VS Goodness(Without waiting in rq time): 1050ms VS 1049ms*


*Quicker execution expected when waiting in runqueue time is not taken because we don't multiply anymore
 with a fraction that the numerator is bigger than the denominator so the Goodness value is reduced.
 (max (1+waitingInRQ(all_processes))>=1+waitingInRQ(current_process)).


