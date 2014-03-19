
a=2.5;
b=25;
c=20;
d=20;
e=5;
f=20;
g=15;
x=1.5;
y = .75;
p = [
[0, a],   [b, 0], [b+c, 0], [2*b+c, a],   [2*b+c, a+d],     [e, g+f],   [0, g], 
[x, a+x], [b, x], [b+c, x], [2*b+c-x, a+x], [2*b+c-x, a+d-x], [e+x, g+f-x], [x, g]];
p2 = [
[y, a+y], [b, y], [b+c, y], [2*b+c-y, a+y], [2*b+c-y, a+d-y], [e+y, g+f-y], [y, g], 
[x, a+x], [b, x], [b+c, x], [2*b+c-x, a+x], [2*b+c-x, a+d-x], [e+x, g+f-x], [x, g]];
linear_extrude(height=7){
polygon(points=p,
 paths=[[0, 1, 2, 3, 4, 5, 6], [7, 8, 9, 10, 11, 12, 13]]);

}
linear_extrude(height=71){
polygon(points=p,
 paths=[[0, 1, 8, 7, 13, 12, 11, 10, 9, 2, 3, 4, 5, 6]]);

}
linear_extrude(height=73){
polygon(points=p2,
 paths=[[0, 1, 8, 7, 13, 12, 11, 10, 9, 2, 3, 4, 5, 6]]);

}
linear_extrude(height=x){
polygon(points=p,
 paths=[[0, 1, 2, 3, 4, 5, 6]]);
}