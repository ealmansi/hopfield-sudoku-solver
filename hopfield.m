sudokuSize = 2;
A = lirows(sudoku2ExactCover(sudokuSize));
n = size(A,1);
m = size(A,2);
A_ext = [A, eye(n)];
m_ext = n + m;

% verifico que las restricciones sean LI
assert(rank(A_ext) == size(A_ext,1));

% parámetros
cantIteraciones = 100000;
c = 0;
Tp = 100;
alfa = ((10^-3)/(Tp))^(1/cantIteraciones);

% obtengo Top, Iop
Top = -2*A*A';%zeros(m_ext);
Iop = 4*ones(m_ext,1);%zeros(m_ext,1);%[ones(m,1);zeros(n,1)];

% obtengo Tval, Ival en función de A
A_ext_pinv = pinv(A_ext);
Tval = eye(m_ext) - A_ext_pinv * A_ext;
Ival = A_ext_pinv * ones(n,1);

% obtengo T e Ib en funcion de los previos y un parámetro "c"
T = Top;% + c * (Tval - eye(m_ext));
Ib = Iop;% + c * Ival;

% hago que T sea simétrica con T <- 1/2 (T + T')
T = 1/2 * (T + T');

% inicializo vector de estado "v" con valores random entre 0-1
v = rand(m_ext,1);


for it=1:cantIteraciones
	% i <- entero random entre 1 y la long de "v"
	i = randi(m_ext);

	% obtengo ui como ui <- (T * v + Ib)i
	ui = T(i,:) * v + Ib(i);

	% actualizo vi con vi <- 1/(1 + exp( -ui / Tp )) , "Tp" parámetro
	v(i) = 1/(1 + exp(-ui / Tp));

	% opcional: disminuyo el valor de "Tp"
	Tp = Tp * alfa;

	% opcional: imprimo E(v)
	% Ev = - 1/2 * v' * T * v - v' * Ib

	% opcional: imprimo norma(v - (Tval * v + Ival))
	% dist = norm(v - (Tval * v + Ival))
end

columnasPisadas = sum(A_ext * v > 1)
filasTomadas = v' * [ones(m,1);zeros(n,1)]
exactCover2Sudoku(sudokuSize, v(1:m))