scc 'auto P = pair<int,float>{1,3.3};  __ P, ++P, P++'
scc 'auto P = tuple<int,float>{1,3.3};  __ P, ++P, P++;'
scc 'vint S{1,2,3}, T;  S>>T; T'
scc 'array<int,3> a; rng|a = 10; a'
scc 'int a[3];  rng|a = 10; a'
scc 'vint a(3);  rng|a = 10; a'
scc 'vint v{1,2};  __ (+v/-v=11);  __ (range(v)=22);  __ (v|rng = 33) ;'
scc 'stack<int> s; s << 1 << 2 << 3;  int x;  s >> x; _ x, s++; __ "", !s--;'
scc 'queue<int> q; q << 1 << 2 << 3;  int x;  x << q;  __ x, ++q, q++;   __ !--q;'
