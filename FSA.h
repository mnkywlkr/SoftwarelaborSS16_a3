
// Endliche Automaten

#ifndef __FSA__
#define __FSA__

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Transition;
class NFA;
class FSA;



class Transition {
 private:
  int from; 
  char c; 
  int to;
  bool epsilon;
 public:

  Transition(int _from, int _to) {
    from = _from; to = _to; 
    epsilon = true;
  } 
  Transition(int _from, char _c, int _to) {
    from = _from; c = _c; to = _to; 
    epsilon = false;
  } 
  bool isEpsilonTransition() { return epsilon; }
  int toState() { return to; }
  bool trigger(int from, char c) {
    return (!epsilon && from == this->from && c == this->c);
  }
  bool trigger(int from) {
    return (epsilon && from == this->from);
  }
};

class NFA {
 private:
  vector<Transition> ts;
  int init;
  vector<int> final;

 public:
  NFA(vector<Transition> _ts, int _init, vector<int> _final) {
    ts = _ts;
    init = _init;
    final = _final;
  }
  NFA(vector<Transition> _ts, int _init, int _final) {
    ts = _ts;
    init = _init;
    final.push_back(_final);
  }
  vector<Transition> getTransitions() { return ts; }
  int getInitial() { return init; }
  vector<int> getFinals() { return final; }

  friend class FSA;
};

class FSA : public NFA {
 private:
  vector<int> current;
  void closure();
 public:
  FSA(NFA fsa) : NFA(fsa.ts,fsa.init,fsa.final) {
    current.push_back(init);
    closure();
  }
  void reset();
  void step(char c);
  bool isFinal();
  bool run(string s);

};

void FSA::reset() {
    current.clear();
    current.push_back(init);
    closure();	
}

bool FSA::isFinal() {
    for(int i=0; i < final.size(); i++) {
      if(find(current.begin(),current.end(),final[i]) != current.end())
	return true;
    }     
    return false;
}

void FSA::closure() {
  // Ihre Aufgabe
  
  /** 	1.schritt: iterieren ueber die Menge der Transitionen
  *		
  *		2.Schritt: alle Epsilon herausfiltern mit Hilfsmethode 
  *			isEpsilonTransition() von der Klasse Transition
  * 	
  *		3.Schritt: Testen, ob ein Zustand erreicht wird
  *			mit trigger() und toState() der Klasse Transition.
  *			Angenommen t ist eine Transition und q ein Zustand(ein Integer).
  *			falls 
  *			t eine epsilon Transition ist und Transition t als Startpunkt den Zustand q hat.
  *			=> dann 
  *			(t.isEpsilonTransition() && t.trigger()) = wahr
  *			Via t.toState(q) kann man den Endpunkt der Epsilon Transition berechnen.
  *		
  *		4.Schritt: Um zu ueberpruefen, ob ein Element in einem Vektor ist,
  *			benutzen wir find().
  *			Angenommen v ist vom typ vector<int> und q ist ein Zustand,
  *			dann 
  *			falls q nicht in v enthalten ist
  *			=>
  *			(find(v.begin(),v.end(),q) == v.end()) = wahr 
  *  
  */
  
  //iterieren ueber die Menge der Transitionen
  for (int i=0; i < ts.size();i++) {
	  //epsilon herausfiltern
		if(ts[i].isEpsilonTransition()) {
			  //Durch current states durchgehen
			for (int j = 0; j < current.size(); j++) {
				  //Wenn Uebergang von current ist
				if (ts[i].trigger(current[j])) {
					//Wenn wir den toState noch nicht im current state haben
					if (find(current.begin(), current.end(), ts[i].toState()) == current.end()) {
					//Fuege neues Ergebnis der Transition current hinzu.
					current.push_back(ts[i].toState());
					closure();
					}
				}
			}
		}
	}
}

void FSA::step(char c) {
    vector<int> next;
    for(int i=0; i < ts.size(); i++) {
      for (int j=0; j < current.size(); j++) {
        if(ts[i].trigger(current[j],c))
	  next.push_back(ts[i].toState());
      }
    }
    current = next;
    closure();
}

bool FSA::run(string s) {
  reset();
  for(int i=0; i < s.length(); i++) {
    step(s[i]);
  }
  return isFinal();
}

#endif // __FSA__
