# ifndef CHECKER_CPP
# define CHECKER_CPP

# include <string>
# include "Scope.h"
# include "Type.h"
# include "lexer.h" // For the report() function

Scope *globalScope;
Scope *currentScope;

void error(int errorType, std::string name) {
	switch (errorType) {
		case 1:	report("E1. redefinition of '%s'", name);
			break;
		case 2:	report("E2. redeclaration of '%s'", name);
			break;
		case 3:	report("E3. conflicting types for '%s'", name);
			break;
		case 4:	report("E4. '%s' undeclared", name);
			break;
		case 5:	report("E5. '%s' has void type", name);
			break;
		default: report("Error not defined");
	}
}

/************************
 * Scoping
 * *********************/

void openScope() {
	Scope *scope = new Scope(currentScope);

	if (globalScope == NULL) 
		globalScope = scope;

	currentScope = scope;	
}

void closeScope() {
	Scope *higherScope = currentScope->higherScope();
	delete currentScope;
	currentScope = higherScope;
}

/*************************
 * Variables
 ************************/

void declareVar(Symbol *symbol) {
	if (currentScope == globalScope) {
		// Check for error E3	
		Symbol *prevDecl = currentScope->findByName(symbol->name());
		if (prevDecl && prevDecl->type() != symbol->type())
			error(3, symbol->name());
	} else {
		if (currentScope->find(symbol) != NULL) {
			// E2 Error
			error(2, symbol->name());
		}
	}

	// Remove old declarations before inserting
	currentScope->remove(symbol);
	currentScope->insert(symbol);
}

/* Array declaration */
void declareVar(int spec, unsigned indirection, std::string name, unsigned length) {
	Type newType(ARRAY, spec, indirection, length);
	declareVar(new Symbol(name, newType));
}

/* Scalar declaration */
void declareVar(int spec, unsigned indirection, std::string name) {
	Type newType(SCALAR, spec, indirection);
	declareVar(new Symbol(name, newType));
}

/* Checking to see if a variable has been declared in current or enclosing scopes. */
void checkVar(Symbol *symbol) {
	if (currentScope->lookup(symbol) == NULL)	
		error(4, symbol->name());
	if (symbol->type().specifier() == VOID)
		error(5, symbol->name());
}

/*************************
 * Functions
 ************************/

// NOTE: Still have to figure out how to handle parameters.
void defineFunc(int spec, unsigned indirection, std::string name) {
	Type newType(FUNCTION, spec, indirection);
	Symbol *func = new Symbol(name, newType, true);

	Symbol *prevDec = globalScope->findByName(name);
	
	if (prevDec->type() != func->type()) {
		error(3, name);		
	}

	// If function is already defined, throw an error.
	if (prevDec->defined()) {
		error(1, name);
	}

	// need to remove any of this function's declarations
	globalScope->remove(prevDec);
	
	globalScope->insert(func);	
}

void declareFunc(int spec, unsigned indirection, std::string name) {
	Type newType(FUNCTION, spec, indirection);
	Symbol *func = new Symbol(name, newType);

	Symbol *prevDec = globalScope->findByName(name);

	if (prevDec->type() != func->type()) {
		error(3, name);
	} else if (*prevDec != *func) {
		// Redeclarations must be the same
		error(2, name);
	}
	
	// Keep defined functions
	if (prevDec->defined()) 
		return;

	// need to remove any previous declarations
	globalScope->remove(prevDec);

	currentScope->insert(func);
}

void checkFunc(int spec, unsigned indirection, std::string name) {
	Symbol *func = globalScope->findByName(name); 
	if (func == NULL) {
		// Implicitly declare function
		declareFunc(spec, indirection, name);
		return;
	}
}

# endif
