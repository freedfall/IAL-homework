/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
	** Implementujte proceduru pro převod infixového zápisu matematického výrazu
	** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
	** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
	** se o řešení tohoto příkladu nepokoušejte.
	**
	** Implementujte následující funkci:
	**
	**    infix2postfix ... konverzní funkce pro převod infixového výrazu
	**                      na postfixový
	**
	** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
	**    
	**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
	**    doOperation .... zpracování operátoru konvertovaného výrazu
	**
	** Své řešení účelně komentujte.
	**
	** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
	** Proto zde používáme pojem funkce i pro operace, které by byly
	** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
	** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	//end cycle when stack is empty
	while (!Stack_IsEmpty(stack)){
		char operator;
		Stack_Top(stack, &operator);

		if (operator == '('){
			Stack_Pop(stack);
			return;
		}

		else{
			postfixExpression[(*postfixExpressionLength)++] = operator;
			Stack_Pop(stack);
		}
	}
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */


void doOperation(Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength) {
    // Define operator priorities
    int priority[256] = {0}; // Initialize to 0
    priority['+'] = priority['-'] = 1; // Priority 1 for '+' and '-'
    priority['*'] = priority['/'] = 2; // Priority 2 for '*' and '/'

    int topPriority;

    while (!Stack_IsEmpty(stack)) {
        char top;
        Stack_Top(stack, &top);
        topPriority = priority[(int)top]; // Cast 'top' to int

        if (priority[(int)c] <= topPriority) { // Cast 'c' to int
            Stack_Pop(stack);
            postfixExpression[(*postfixExpressionLength)++] = top;
        } else {
            break;
        }
    }

    Stack_Push(stack, c);
}



/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */

char *infix2postfix( const char *infixExpression ) {
	Stack _stack;
	Stack *stack = &_stack;
	Stack_Init(stack);
	if (stack->array == NULL)
		return NULL;
	
	char *postfixExpression = (char *)malloc(sizeof(MAX_LEN * sizeof(char)));

	if (postfixExpression == NULL)
		return NULL;
	
	unsigned postfixExpressionLength = 0;

	// Loop through each character in the input infix expression
	for (int i = 0; infixExpression[i] != '\0'; i++){
		char c = infixExpression[i];

        // If the character is a digit or a variable, add it to the postfix expression
		if ( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
			postfixExpression[(postfixExpressionLength)++] = c;

        // If the character is an opening parenthesis '(', push it onto the stack
		else if (c == '(')
			Stack_Push(stack, c);

        // If the character is a closing parenthesis ')', call untilLeftPar function to delete
		else if (c == ')')
			untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
		
        // If the character is an operator (+, -, *, /), process operators based on priority			
		else if (c == '+' || c == '-' || c == '*' || c == '/')
			doOperation(stack, c, postfixExpression, &postfixExpressionLength);
		
        // If the character is '=', process and empty the stack
		else if (c == '=') {
			while (!Stack_IsEmpty(stack)) {
				Stack_Top(stack, &c);
				Stack_Pop(stack);
				postfixExpression[postfixExpressionLength] = c;
				postfixExpressionLength++;
			}

            // Add '=' to the postfix expression
			postfixExpression[postfixExpressionLength] = '=';
			postfixExpressionLength++;
		} 
        // If the character is neither an operator nor a digit/variable, add it to the postfix expression
		else {
			postfixExpression[postfixExpressionLength] = c;
			postfixExpressionLength++;
		}
	}
    // Null-terminate the postfix expression
	postfixExpression[postfixExpressionLength] = '\0';

	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push(Stack *stack, int value) {
    // Push the individual bytes of the integer value onto the stack
    for (int i = 0; i < 4; i++) {
        Stack_Push(stack, (char)((value >> (i * 8)) & 0xFF));
    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop(Stack *stack, int *value) {
    char bytes[4];

    // Pop the individual bytes from the stack and reconstruct the integer value
    for (int i = 3; i >= 0; i--) {
        Stack_Top(stack, &bytes[i]);
        Stack_Pop(stack);
    }

    *value = *(int *)bytes;
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */

VariableValue *getVariableValue(char variableName, VariableValue variableValues[], int variableValueCount) {
	for(int i = 0; i < variableValueCount; i++) {
		if (variableName == variableValues[i].name) {
			return &variableValues[i];
		}
	}
	return NULL;
}

int calculate(char operand, int operator1, int operator2) {
	switch (operand)
	{
	case '+':
		return operator1 + operator2;
	case '-':
		return operator1 - operator2;
	case '*':
		return operator1 * operator2;
	case '/':
		return operator1 / operator2;
	}

	return 0;
}



bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char *postfixExpression = infix2postfix(infixExpression);
	if (postfixExpression == NULL)
		return false;

    // Initialize a stack for intermediate results
	Stack _stack;
	Stack *stack = &_stack;
	Stack_Init(stack);

    // Check if stack initialization was successful
	if (stack->array == NULL) {
		free(postfixExpression);
		return false;
	}

	char c;
	int value1;
	int value2;

	VariableValue *charData;

    // Loop through each character in the postfix expression
	for (int i = 0; postfixExpression[i] != '\0'; i++){
		c = postfixExpression[i];
		if (c == '+' || c == '-' || c == '*' || c == '/') {
            // Operator encountered, pop two values from the stack and perform the operation
			expr_value_pop(stack, &value1);
			expr_value_pop(stack, &value2);
			//Division by zero
			if (c == '/' && value1 == 0){
				free(postfixExpression);
				Stack_Dispose(stack);
				return false;
			}
			expr_value_push(stack, calculate(c, value2, value1));
		} 
		else if (c == '=') {
            // Expression evaluation complete, pop the final result and clean up
			expr_value_pop(stack, value);
			free(postfixExpression);
			Stack_Dispose(stack);
			return true;
		} 
		else { 
            // Operand or variable encountered, push its value onto the stack
			charData = getVariableValue(c, variableValues, variableValueCount);

            // Check if the variable value exists
			if (charData == NULL) {
				free(postfixExpression);
				Stack_Dispose(stack);
				return false;
			}

			expr_value_push(stack, charData->value);
		}
	}

    // Clean up and return false if the evaluation did not complete as expected
	free(postfixExpression);
	Stack_Dispose(stack);
	return false;
}


/* Konec c204.c */
