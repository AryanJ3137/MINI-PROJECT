/* ---------------- Financial calculators ---------------- */

void loanEMI_calculator() {
    float P, R;
    int Y;
    printf("\n--- Loan EMI Calculator ---\n");
    printf("Loan principal (\u20b9): "); if (scanf("%f", &P) != 1) { while (getchar() != '\n'); return; }
    printf("Annual rate (%%): "); if (scanf("%f", &R) != 1) { while (getchar() != '\n'); return; }
    printf("Loan duration (years): "); if (scanf("%d", &Y) != 1) { while (getchar() != '\n'); return; }

    float monthlyRate = R / (12.0f * 100.0f);
    int months = Y * 12;
    if (monthlyRate <= 0.0f) {
        printf("Monthly EMI: %.2f\nTotal payment: %.2f\nTotal interest: %.2f\n",
               P / months, P, 0.0f);
        return;
    }
    float EMI = (P * monthlyRate * powf(1 + monthlyRate, months)) / (powf(1 + monthlyRate, months) - 1);
    float total = EMI * months;
    printf("Monthly EMI: \u20b9%.2f\nTotal payment: \u20b9%.2f\nTotal interest: \u20b9%.2f\n", (double)EMI, (double)total, (double)(total - P));
}

void SIP_calculator() {
    float monthly, annualReturn;
    int years;
    printf("\n--- SIP Calculator ---\n");
    printf("Monthly investment (\u20b9): "); if (scanf("%f", &monthly) != 1) { while (getchar() != '\n'); return; }
    printf("Expected annual return (%%): "); if (scanf("%f", &annualReturn) != 1) { while (getchar() != '\n'); return; }
    printf("Duration (years): "); if (scanf("%d", &years) != 1) { while (getchar() != '\n'); return; }

    int months = years * 12;
    float monthlyRate = annualReturn / (12.0f * 100.0f);
    float future;
    if (monthlyRate <= 0.0f) {
        future = monthly * months;
    } else {
        future = monthly * ((powf(1 + monthlyRate, months) - 1) / monthlyRate) * (1 + monthlyRate);
    }
    printf("Estimated future value after %d years: \u20b9%.2f\n", years, (double)future);
}

void FD_calculator() {
    float P, r;
    int years;
    printf("\n--- Fixed Deposit Calculator ---\n");
    printf("Principal (\u20b9): "); if (scanf("%f", &P) != 1) { while (getchar() != '\n'); return; }
    printf("Annual rate (%%): "); if (scanf("%f", &r) != 1) { while (getchar() != '\n'); return; }
    printf("Years: "); if (scanf("%d", &years) != 1) { while (getchar() != '\n'); return; }
    float maturity = P * powf(1 + r / 100.0f, years);
    printf("Maturity amount: \u20b9%.2f\n", (double)maturity);
}

/* ---------------- Advanced Smart Investment Advisor ----------------
   Uses age, monthly income, risk appetite, duration to suggest allocations,
   emergency fund, SIP suggestions and projected returns.
*/
void investmentAdvisor_advanced(int idx) {
    printf("\n--- Smart Investment Advisor (Advanced) ---\n");

    int age;
    float monthlyIncome;
    int riskChoice; /* 1 Low, 2 Medium, 3 High */
    int years;

    printf("Enter your age (years): "); if (scanf("%d", &age) != 1) { while (getchar() != '\n'); return; }
    printf("Enter monthly disposable income for investments (\u20b9): "); if (scanf("%f", &monthlyIncome) != 1) { while (getchar() != '\n'); return; }
    printf("Select risk appetite: 1) Low  2) Medium  3) High : "); if (scanf("%d", &riskChoice) != 1) { while (getchar() != '\n'); return; }
    printf("Investment duration (years): "); if (scanf("%d", &years) != 1) { while (getchar() != '\n'); return; }

    if (years <= 0) { printf("Invalid duration\n"); return; }
    if (monthlyIncome < 0) monthlyIncome = 0;

    /* Emergency fund recommendation */
    int emergencyMonths = 6; /* default 6 months */
    if (riskChoice == 1) emergencyMonths = 9;
    if (age > 55) emergencyMonths = 9;
    float emergencyFund = emergencyMonths * monthlyIncome;

    /* base equity allocation by age and risk:
       rule-of-thumb style: younger -> more equity
    */
    int baseEquityPct;
    if (age < 30) baseEquityPct = 70;
    else if (age < 45) baseEquityPct = 60;
    else if (age < 60) baseEquityPct = 40;
    else baseEquityPct = 30;

    /* adjust by risk appetite */
    int equityPct = baseEquityPct;
    if (riskChoice == 3) equityPct += 10;
    if (riskChoice == 1) equityPct -= 10;

    if (equityPct > 90) equityPct = 90;
    if (equityPct < 10) equityPct = 10;

    int debtPct = 100 - equityPct;
    int goldPct = 5; /* small allocation to gold */
    if (equityPct <= 40) goldPct = 10;

    /* Slight redistribution */
    int eq = equityPct - goldPct;
    int de = debtPct;
    if (eq < 0) { eq = equityPct; goldPct = 0; }

    /* expected return assumptions (conservative estimates) */
    float eqReturn = 0.12f;    /* 12% p.a equity */
    float debtReturn = 0.06f;  /* 6% p.a debt/FD */
    float goldReturn = 0.06f;  /* 6% p.a gold */

    /* Projected returns starting from current balance + monthly SIP from disposable income */
    float currentBal = decrypt(bank[idx].balance);
    float monthlySIP = monthlyIncome; /* use all disposable amount as SIP (suggestion) */

    /* Compute future value with monthly SIP for 'years' assuming blended return */
    float blendedReturn = ((eq * (eqReturn)) + (de * (debtReturn)) + (goldPct * (goldReturn))) / 100.0f;
    int months = years * 12;
    float monthlyRate = blendedReturn / 12.0f;
    float future = currentBal * powf(1 + blendedReturn, years);
    if (monthlyRate > 0.0f) {
        future += monthlySIP * ((powf(1 + monthlyRate, months) - 1) / monthlyRate) * (1 + monthlyRate);
    } else {
        future += monthlySIP * months;
    }

    /* SIP suggestion amount (practical) */
    float suggestedSIP = monthlyIncome * 0.6f; /* recommend putting 60% of provided disposable into SIP */
    if (suggestedSIP < 0) suggestedSIP = 0;

    /* Present suggestions */
    printf("\n--- Personalized Recommendation ---\n");
    printf("Emergency Fund Suggestion: \u20b9%.2f (approx. %d months of income)\n", (double)emergencyFund, emergencyMonths);
    printf("Target Asset Allocation (approx):\n");
    printf("  Equity: %d%%\n  Debt/FD: %d%%\n  Gold: %d%%\n", eq, de, goldPct);
    printf("Suggested monthly SIP: \u20b9%.2f (you provided \u20b9%.2f disposable)\n", (double)suggestedSIP, (double)monthlyIncome);
    printf("Projected portfolio value after %d years (with suggested SIP): \u20b9%.2f\n", years, (double)future);
    printf("\nNotes:\n - Rebalance yearly.\n - Keep emergency fund in liquid savings/FD.\n - Start SIPs early to benefit from compounding.\n");

    /* Save to history with key details */
    char hist[200];
    snprintf(hist, sizeof(hist), "InvAdvice: age%d risk%d yrs%d SIP%.0f", age, riskChoice, years, suggestedSIP);
    addHistory(idx, hist);
    saveToFile();
}

// TEST CASE 
/* Scenario: A user logs in and uses:
1. Investment Advisor
2. SIP Calculator
3. FD Calculator
4. Loan EMI Calculator 

Step 1: Login
Enter Account Number: 101
Enter PIN: 1234
  

Step 2: Investment Advisor (Advanced)
Choose option: 7
Input: Age: 25
       Monthly income: 20000
       Risk: 3 (High)
       Duration: 10 years
Output:  
Emergency Fund: ₹120000 (6 months)
Allocation: Equity: ~75%
            Debt: ~25%
            Gold: ~5%
Suggested SIP: ₹12000
Projected value after 10 years: ₹ (large value ~25–30 lakhs depending on calc)
Notes: - Rebalance yearly
       - Keep emergency fund
       - Use compounding


Step 3: SIP Calculator
Choose: 8
Input: Monthly investment: 5000
       Return: 12%
       Years: 10
Output: Future Value: ₹~11,61,000


Step 4: FD Calculator
Choose: 9
Input: Principal: 100000
       Rate: 7%
       Years: 5
Output: Maturity Amount: ₹~1,40,255


Step 5: Loan EMI Calculator
Choose: 10
Input: Loan: 500000
       Rate: 10%
       Years: 5
Output: Monthly EMI: ₹~10624
        Total Payment: ₹~6,37,440
        Total Interest: ₹~1,37,440 */ 


/* My portion of the project deals with financial intelligence features such as EMI calculation, 
   SIP planning, FD growth, and an advanced investment advisor. It utilizes mathematical models 
   such as compound interest and portfolio allocation strategies. */ 

/* This is a test case for all the functionalities, including normal and edge cases. 
   It shows the way the system helps the user in investment planning, calculates the return on 
   investment and provides personalized financial advice based on the age, 
   income level, and risk-taking capacity of the individual. */ 
