all:
	g++ vmsim.cpp -std=c++11 -o vmsim 
	g++ vmgenU.cpp -std=c++11 -o vmgenU
	g++ vmgenWS.cpp -std=c++11 -o vmgenWS
