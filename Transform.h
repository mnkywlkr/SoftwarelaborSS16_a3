
// Ein moeglicher Rahmen fuer Aufgabe 4, zweite Teilaufgabe,
// uebersetze regulaeren Ausdruck in einen NFA.
// Der Einfachheit in ein .h File gepackt.


#include <iostream>
#include <vector>

using namespace std;

#include "FSA.h"
#include "RE.h"


int nameSupply;

void init() {
  nameSupply = 0;
}

int fresh() {
  return nameSupply++;
}


// Macht die eigentliche Arbeit
NFA transformWorker(RE *r);

// Schnittstelle fuer Benutzer
// Ruecksetzen des "name supplies" zur Generierung von eindeutigen Zustaenden
// Aufruf von transform2
NFA transform(RE *r) {
  init();
  return transformWorker(r);
}


// Wir liefern einen NFA zurueck mit genau einem Start und
// genau einem Stop(end)zustand.
NFA transformWorker(RE *r) {
  vector<Transition> ts;
  int start, stop;

  switch(r->ofType()) {

  case EpsType: {
	// TODO
	
	start = fresh();
    stop = fresh();
	//transition
	ts.push_back(Transition(start, stop));
	return NFA(ts, start, stop);
  }
  
  case ChType: {
  // TODO
	
	start = fresh();
    stop = fresh();
	//transition
	ts.push_back(Transition(start,((Ch*)r)->getChar(),stop));
	return NFA(ts, start, stop);
  }
  
  case StarType: {
	  //Sollte jetzt richtig sein.
	NFA n = transformWorker(((Star*)r)->getRE());
	
	start = fresh();
    stop = fresh();
	int n_start = n.getInitial();
	int n_stop = n.getFinals()[0];

	
	//Transitionen von bereits existierendem Automat
	vector<Transition> t_a = n.getTransitions();
	  
	//fuege alle Transitionen t_a
	ts.insert(ts.begin(),t_a.begin(),t_a.end());
	
	//epsilon transition von start nach n_start
	ts.push_back(Transition(start,n_start));
	//epsilon transition von start nach stop
	ts.push_back(Transition (start,stop));
	//epsilon transition von n_start nach n_stop
	ts.push_back(Transition (n_stop,n_start));
	//epsilon transition von n_stop nach stop;
	ts.push_back(Transition(n_stop,stop));
	/*
	* CHANGED ************************************
	*
	*
	**/
	//epsilon transition von stop nach start
	//fuer alle stop -> start kombinationen
	//ts.push_back(Transition(stop, start));
	//ts.push_back(Transition(n_stop, n_start));
	//ts.push_back(Transition(n_stop, start));


	
    return NFA(ts, start, stop);
  }
  
  case ConcType: {
  // TODO Ich glaube richtig!
	Conc* r2 = (Conc*) r;
    // 1. Baue NFAs der linken und rechten Alternative
     NFA n1 = transformWorker(r2->getLeft());
     NFA n2 = transformWorker(r2->getRight());
	
	 int n1_start = n1.getInitial();
     int n1_stop  = n1.getFinals()[0];
     int n2_start = n2.getInitial();
     int n2_stop  = n2.getFinals()[0];
	 start = n1_start;
	 stop  = n2_stop;
	 
	 
	//Transitionen
	 vector<Transition> t1 = n1.getTransitions();
     vector<Transition> t2 = n2.getTransitions();

	
    ts.insert(ts.end(),t1.begin(),t1.end());
    ts.insert(ts.end(),t2.begin(),t2.end());
    ts.push_back(Transition(n1_stop, n2_start));

    return NFA(ts, start, stop);
  }
	
  // Phi: Akzeptiert kein Wort
  // NFA besteht aus einem Start und Stopzustand und *keiner* Transition
  case PhiType: {
    start = fresh();
    stop = fresh();
    return NFA(ts, start, stop);
  }
  case AltType: {
     Alt* r2 = (Alt*) r;

     // 1. Baue NFAs der linken und rechten Alternative
     NFA n1 = transformWorker(r2->getLeft());
     NFA n2 = transformWorker(r2->getRight());

     // 2. Generieren neuen Start-/Stopzustand.
     //    Sammle Start-/Stopzustaende von n1 und n2
     // N.B. Annahme: finals besteht aus genau einem Zustand
     start = fresh();
     stop = fresh();
     int n1_start = n1.getInitial();
     int n1_stop  = n1.getFinals()[0];
     int n2_start = n2.getInitial();
     int n2_stop  = n2.getFinals()[0];


     // 3. Sammle Transitionen auf von n1 und n2.
     //    Verbinde neuen Start-/Endzustand mit alten Start-/Endzustaenden.
     vector<Transition> t1 = n1.getTransitions();
     vector<Transition> t2 = n2.getTransitions();

     ts.insert(ts.end(),t1.begin(),t1.end());
     ts.insert(ts.end(),t2.begin(),t2.end());
     ts.push_back(Transition(start, n1_start));
     ts.push_back(Transition(start, n2_start));
     ts.push_back(Transition(n1_stop, stop));
     ts.push_back(Transition(n2_stop, stop));

     return NFA(ts,start,stop);
  }
      


  } // switch


} // transformWorker



