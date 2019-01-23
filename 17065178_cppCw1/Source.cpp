#include <iostream>
///////////////////////////////////////////////
///			      NODE CLASS				///
///////////////////////////////////////////////
class Node{
public:
	Node(int value, Node* nextptr = nullptr, Node* prevptr = nullptr, int currentpriority = 0);//constructor for the Node class which takes in the default values
	int getVal(void); //returns the process ID value of the current node
	Node* getNext(void); //returns the next node's location 
	Node* getPrev(void); //returns the previous node's location
	void setVal(int value); //sets the prcess ID value of the current node
	void setPrev(Node* prevptr); //sets the previous node's value to the passed in value
	void setNext(Node* nextptr); //sets the next node's value to the passed in value
	int getPriority(void); //returns the priority value of the current node
	void setPriority(int priority); //sets the priority value of the current node to the passed in value
private:
	Node* next; //pointer to the next node's location
	Node* prev; //pointer to the previous node's location
	int priority; //integer used to determine the priority of the task
	int value; //integer used to represent the process ID
};


Node::Node(int value, Node * nextptr, Node * prevptr, int currentpriority) { //constructor for the Node class which takes in the default values
	next = nextptr;
	prev = prevptr;
	priority = currentpriority;
	this->value = value;
}
int Node::getVal(void) { //returns the process ID value of the current node
	return value;
}
Node * Node::getNext(void) {//returns the next node's location 
	return next;
}
Node * Node::getPrev(void) {//returns the previous node's location
	return prev;
}
void Node::setVal(int value) {//sets the prcess ID value of the current node
	value = this->value;
}
void Node::setPrev(Node * prevptr) {//sets the previous node's value to the passed in value
	prev = prevptr;
}
void Node::setNext(Node * nextptr) {//sets the next node's value to the passed in value
	next = nextptr;
}
int Node::getPriority(void) {//returns the priority value of the current node
	return priority;
}
void Node::setPriority(int priority) {//sets the priority value of the current node to the passed in value
	this->priority = priority;
}

///////////////////////////////////////////////
///			      QUEUE CLASS				///
///////////////////////////////////////////////
class Queue{
public:
	Queue(void); //default constructor
	~Queue(void); //default desructor
	void Enqueue(int i, int priority = 0); //used to instert data to the queue
	int Dequeue(void); //used to remove data from the queue
protected:
	Node* back; //pointer to the last item's location of the queue
	Node* front; //pointer to the first item's location of the queue 
private:
	virtual Node* NodeDequeue(void); //returns the pointer of the node's location
};

Queue::Queue(void) { //default constructor
	front = back = NULL;
}

Queue::~Queue(void) {//default destructor
	while (front != NULL)
		delete NodeDequeue();
}

void Queue::Enqueue(int i, int priority) {		//used to instert data to the queue
	if (priority < 0 || priority > 10)			//checks for the priority range
		throw ("Priority is out of range");		//if it's either below 0 or above 10, it throws an exception
	Node* tmp = nullptr;						//sets the temporary node to a nullpointer
	tmp = new Node(i, back, nullptr, priority);	//sets the temporary node to a new Node with process ID being the given 'i' variable, the next Node being the bottom of the queue(back), the previous node being a nullpointer, and the priority being the given 'priority' variable
	back = tmp;									//makes the bottom of the queue equal to the temporary Node
	if (front == nullptr) front = back;			//checks if the top of the queue is empty, if it is-> set it's value to the bottom of the queue
	else {										// if it isn't-> it sets the temporary node to be the one before the last one
		tmp = back->getNext();
		tmp->setPrev(back);
	}
}

Node * Queue::NodeDequeue(void) {
	Node* tmp = front;									//makes new temporary Node and set it's value to the top of the queue
	if (front != nullptr) {								//if the top of the queue is not empty -> set the first element to be the second element
		front = front->getPrev();						//if the top of the queue is not empty -> sets the first element's pointer to a null pointer
		if (front != nullptr) front->setNext(nullptr);
	}
	return tmp;											//returns the temporary pointer
}

int Queue::Dequeue(void) {
	Node* tmp = NodeDequeue(); //makes new temporary Node pointer and set it to the pointer returned by the NodeDequeue()
	int ret = 0;
	if (tmp != nullptr)			//if queue is not empty->sets the return value to be the process id of the temporary Node
		ret = tmp->getVal();
	else
		throw ("Queue Empty"); //if queue is empty, throw an exception
	if (front == nullptr)		//if queue is empty->sets the bottom of the queue to be equal to the front of the queue
		back = front;
	delete tmp;					//deletes the temporary Node pointer
	return ret;					//returns the return value
}


///////////////////////////////////////////////
///			      STACK CLASS				///
///////////////////////////////////////////////
class Stack{
public:
	Stack(void); //default constructor of Stack class
	~Stack(void); //default desructor of Stack class
	void Push(int value);//used to instert an element at the top of the stack
	Node* NodePop(void);//used to remove the top element of the stack
	int Pop(void);//returns the top element's value
private:
	Node* top; //pointer to the top of the stack
};

Stack::Stack(void) { //constructor
	top = nullptr; //sets the top of the stack pointer to be a null pointer by default
}

Stack::~Stack(void) { //destructor
	while (top != nullptr) //while the top of the stack is not empty
		delete NodePop();	//delete every Node by calling NodePop() function
}

void Stack::Push(int value) { //used to instert an element at the top of the stack
	Node* tmp = new Node(value, top); //makes temporary Node pointer with the process id being the given 'value' and next pointer being the Node on top of the queue
	top = tmp;							//set the temporary Node's location to be on top of the stack
}

Node * Stack::NodePop(void) { //used to remove the top element of the stack
	Node* tmp = top; //makes temporary Node pointer to the top Node
	if (top != nullptr) { //if stack is not empty
		top = top->getNext();	//set the top pointer to the next pointer in the stack
	}
	return tmp;		//returns the temporary Node pointer
}

int Stack::Pop() {//returns the top element's value
	Node* tmp = NodePop();	//makes a new temporary Node pointer to point at the top of the queue
	if (tmp == nullptr) //checks if stack is empty if it is->throws an exception
		throw ("Stack is empty");
	int ret = tmp->getVal(); //gets the process ID from the temporary Node and set the return value equal to it
	delete tmp;	//deletes temporary Node pointer
	return ret; //return process ID 
}


///////////////////////////////////////////////
///			    SCHEDULER CLASS				///
///////////////////////////////////////////////
class Scheduler : public Queue{
private:
	Node* NodeDequeue(void); //overrides the NodeDequeue method from it's parent class(Queue) and sorts it's elements based on their priority
};


Node * Scheduler::NodeDequeue(void){
	Node* iterator = this->front;
	Node* tmp = iterator;
	if (this->front == nullptr)	{ //check queue is not empty
		std::cout << "Queue is empty" << std::endl;
		return nullptr;
	}
	while (iterator != nullptr) { //loop through queue, starting at front and working backwards
		if (iterator->getPriority() > tmp->getPriority())
			tmp = iterator;
		iterator = iterator->getPrev();
	}
	if (tmp == this->front){//checks if highest priority is the front
		this->front = tmp->getPrev();//sets front to one before tmp
		if (this->front != nullptr)//check to see if there is anything left in the queue
			this->front->setNext(nullptr);
	}

	else if (tmp == this->back){	//set the last in queue to second to last
		this->back = tmp->getNext();
		if (this->back != nullptr)	//check to see if there is anything left in the array
			this->back->setPrev(nullptr);
	}

	//tmp is in the middle of the queue
	else{
		tmp->getPrev()->setNext(tmp->getNext());
		tmp->getNext()->setPrev(tmp->getPrev());
	}
	return tmp;
}