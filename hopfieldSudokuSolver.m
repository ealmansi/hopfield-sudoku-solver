sudokuSize = 2;
A = lirows(sudoku2ExactCover(sudokuSize));
n = size(A,1);
m = size(A,2);
A_ext = [A, eye(n)];
m_ext = n + m;

% verify constraints are linearly independent
assert(rank(A_ext) == size(A_ext,1));

% script parameters
verbose = false;
itCount = 100000;
c = 0;
Tp = 100;
alfa = ((10^-3)/(Tp))^(1/itCount);

% define Top, Iop 		[related to the function to minimize]
Top = -2*A*A';%zeros(m_ext);
Iop = 4*ones(m_ext,1);%zeros(m_ext,1);%[ones(m,1);zeros(n,1)];

% define Tval, Ival 		[related to the feasibility of the solutions]
A_ext_pinv = pinv(A_ext);
Tval = eye(m_ext) - A_ext_pinv * A_ext;
Ival = A_ext_pinv * ones(n,1);

% define T, Ib in terms of c 		[c sets the relative weight between both goals]
T = Top;% + c * (Tval - eye(m_ext));
Ib = Iop;% + c * Ival;

% ensure T is symmetric
T = 1/2 * (T + T');

% init state vector "v" with random values between 0-1
v = rand(m_ext,1);


for it=1:itCount
	% i: random index between 1 and length(v)
	i = randi(m_ext);

	% compute neuron's input
	ui = T(i,:) * v + Ib(i);

	% update it's output in terms of its input and the system's temp (Tp)
	v(i) = 1/(1 + exp(-ui / Tp));

	% decrease Tp geometrically
	Tp = Tp * alfa;

	% optional: display E(v) & display distance to feasability
	if( verbose )
		Ev = - 1/2 * v' * T * v - v' * Ib
		dist = norm(v - (Tval * v + Ival))
	end
end

exactCover2Sudoku(sudokuSize, v(1:m))