# Symbolic-differentiation
C implementation of symbolic differentiation
Simplep rogram that calculates symbolic derivatives and show results in LaTex (step by step)
\documentclass{article}
\begin{document} \begin{center} The original expression 
 \[x + 2\cdot x^{x} + \frac{\sin(x)}{x} + x^{x}\]\[$$After mental calculation: 
$$(x^{x})' = \log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1}\]
\[$$It is well known that: 
$$(2\cdot x^{x})' = x^{x}\cdot 0 + 2\cdot (\log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1})\]
\[$$Using  WolframAlpha we have: 
$$(x + 2\cdot x^{x})' = 1 + x^{x}\cdot 0 + 2\cdot (\log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1})\]
\[$$Every child knows that: 
$$(\sin(x))' = 1\cdot \cos(x)\]
\[$$For a detailed explanation you should purchase the paid version: 
$$(\frac{\sin(x)}{x})' = \frac{x\cdot 1\cdot \cos(x) - \sin(x)\cdot 1}{x\cdot x}\]
\[$$If you are familiar with calculus: 
$$(x + 2\cdot x^{x} + \frac{\sin(x)}{x})' = 1 + x^{x}\cdot 0 + 2\cdot (\log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1}) + \frac{x\cdot 1\cdot \cos(x) - \sin(x)\cdot 1}{x\cdot x}\]
\[$$It's not a secret that: 
$$(x^{x})' = \log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1}\]
\[$$After mental calculation: 
$$(x + 2\cdot x^{x} + \frac{\sin(x)}{x} + x^{x})' = 1 + x^{x}\cdot 0 + 2\cdot (\log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1}) + \frac{x\cdot 1\cdot \cos(x) - \sin(x)\cdot 1}{x\cdot x} + \log(x)\cdot 1\cdot x^{x} + x\cdot 1\cdot x^{x - 1}\]


The final expression
 \[3\cdot \log(x)\cdot x^{x} + 3\cdot x\cdot x^{x - 1} + \frac{x\cdot \cos(x) - \sin(x)}{x^{2}} + 1 \]

\end{center}\end{document}
\end
