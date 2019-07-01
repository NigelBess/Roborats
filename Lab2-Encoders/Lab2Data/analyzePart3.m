clc;
clear all;
close all;
data = csvread('100_178_255_n100.csv');
speeds = {'100','178','255'};
n = 100;
currentData = zeros(n,2);
numTrials = 3;
avgSpeeds = zeros(numTrials,numel(speeds));
figure();
subplot(numTrials,numel(speeds),1);
for j = 1:numTrials
    for i = 1:numel(speeds)
       currentData = data((j-1)*n*numTrials+((i-1)*n)+1:(j-1)*n*numTrials+(i*n),:);
       currentData(:,1) = currentData(:,1)-currentData(1,1);%normalize ticks
       currentData(:,2) = (currentData(:,2)-currentData(1,2))/1000;%s normalize time
       avgSpeeds(j,i) = currentData(end,1)/currentData(end,2);%ticks/s
        subplot(numTrials,numel(speeds),(j-1)*numTrials+i);
        plot(currentData(:,2),currentData(:,1));
        title(['Motor Speed = ',speeds{i}]);
        xlabel('Time (s)');
        ylabel('Encoder Ticks');
    end
end
figure();
plot(data(:,2),data(:,1));