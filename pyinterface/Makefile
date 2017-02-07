all: fastjet.i
	swig -python -c++ -I../include fastjet.i
	$(CXX) -shared `python-config --includes --libs` -DSWIG -I../include `fastjet-config --libs` -fPIC -DPIC fastjet_wrap.cxx -o _fastjet.so

clean:
	rm -f fastjet_wrap.cxx
	rm -f _fastjet.so
