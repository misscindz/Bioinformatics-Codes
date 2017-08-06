clc 
close all
clear all
feature = zeros(60,2);
feature = importdata('data_k_means.mat');
max_f = max(feature);
max_x = max_f(1);
max_y = max_f(2);
min_f = min(feature);
min_x = min_f(1);
min_y = min_f(2);
diff_y = max_y - min_y;
diff_x = max_x - min_x;
centroid_1 = [diff_x/3,diff_y/3];
centroid_2 = [2*(diff_x/3), 2*(diff_y/3)];
centroid_3 = [3*(diff_x/3), 3*(diff_y/3)];
 x = feature(:,1);
 y = feature(:,2);
x_1 = centroid_1(1);
y_1 = centroid_1(2);
x_2 = centroid_2(1);
y_2 = centroid_2(2);
x_3 = centroid_3(1);
y_3 = centroid_3(2);
scatter(x,y);hold on
distance_11 = 0;
distance_21 = 0;
distance_31 = 0;
%scatter(x_1,y_1); hold on
%scatter(x_2,y_2); hold on 
%scatter(x_3,y_3); hold
%on

cluster1 = zeros(20,2);
cluster2 = zeros(20,2);
cluster3 = zeros(20,2);
%dist_cent1 = zeros(60);
j = 1;
p = 1;
q = 1;
center1 = zeros(1,2);
center2 = zeros(1,2);
center3 = zeros(1,2);
%while (center1(1,1) ~= x_1) & (center1(1,2) ~=y_1) & (center2(1,1) ~= x_2)
%& (center2(1,2)~= y_2) & (center3(1,1) ~= x_3) & (center3(1,2)~=y_3) t = 1
for iter = 1:1:100
    
     j = 1;
     p = 1;
     q = 1;
    for i = 1:1:length(feature)
        dist_cen1(i) = sqrt((x_1-x(i))^2+(y_1-y(i))^2);
        dist_cen2(i) = sqrt((x_2-x(i))^2+(y_2-y(i))^2);
        dist_cen3(i) = sqrt((x_3-x(i))^2+(y_3-y(i))^2);
        if dist_cen1(i) < dist_cen2 & dist_cen1(i) < dist_cen3(i) 
         
            cluster1(j,1) = x(i);
            cluster1(j,2) = y(i);
            j= j+1;
        elseif dist_cen2(i) < dist_cen1(i) & dist_cen2(i)< dist_cen3(i)
             cluster2(q,1) = x(i);
             cluster2(q,2) = y(i);
             q= q+1;
        elseif dist_cen3(i) < dist_cen1(i) & dist_cen3(i) < dist_cen2(i)
             cluster3(p,1) = x(i);
             cluster3(p,2) = y(i);
            p= p+1;
         
        end
    end

center1 = mean(cluster1);
center2 = mean(cluster2);
center3 = mean(cluster3);
if (center1(1,1) == x_1) & (center1(1,2) ==y_1) & (center2(1,1) == x_2) & (center2(1,2)~= y_2) & (center3(1,1) == x_3) & (center3(1,2)==y_3)
    break;
else
    x_1 = center1(1);
    y_1 = center1(2);
    x_2 = center2(1);
    y_2 = center2(2);
    x_3 = center3(1);
    y_3 = center3(2);
end
%t=t+1
end
%scatter(x_1,y_1,'+','linewidth',3); hold on
%scatter(x_2,y_2,'+','linewidth',3); hold on
%scatter(x_3,y_3,'+','linewidth',3); hold on
for i = 1:1:length(cluster1)
 distance_11 =distance_11+ sqrt((cluster1(i,1)-x_1)^2 + (cluster1(i,2)-y_1)^2);

end 
for j = 1:1:length(cluster2)
 distance_21 =distance_21+ sqrt((cluster2(j,1)-x_1)^2 + (cluster2(j,2)-y_2)^2);

end 
for z = 1:1:length(cluster3)
 distance_31 =distance_31+ sqrt((cluster3(z,1)-x_3)^2 + (cluster3(z,2)-y_3)^2);

end 
distance_c3 = distance_11+distance_21+distance_31;