close all;

carrier = transpose(audioread('song.wav'));
noise = transpose(audioread('noise.wav'));
noise = noise(1:length(carrier));
carrier = carrier(1:length(noise)) + noise*0.3;
speech = audioread('speech.wav') + (-114 / 32767);
len = (floor(min(length(speech), length(carrier)/6)));

plot(speech, 'black');

speech = speech(1:len);
carrier = carrier(1:len*6);

nFilters = 7;
[aFilts, envLpf] = filters(nFilters, 80, false, false);
figure();
sFilts = filters2(nFilters, 480, false, false);



bandpassed1 = zeros(nFilters, length(speech));
for i = 1:nFilters
    bandpassed1(i,:) = transpose(filter(aFilts(i,:), 1, speech));
end

rect = abs(bandpassed1);

envelopes = zeros(nFilters, length(speech));
for i = 1:nFilters
    envelopes(i,:) = 1.5 * filter(envLpf(i,:), 1, rect(i,:));
%     figure();
%     plot(bandpassed1(i,1000:1500));hold on; plot(envelopes(i,1015:1515));
end

% 
% subplot(2,2,1);
% plot(bandpassed1(2,1000:1100));hold on; plot(envelopes(2,1040:1140));
% title('(a) Channel 2');
% axis([0 100 -inf inf]);
% subplot(2,2,2);
% plot(bandpassed1(3,1000:1100));hold on; plot(envelopes(3,1040:1140));
% title('(b) Channel 3');
% axis([0 100 -inf inf]);
% subplot(2,2,3);
% plot(bandpassed1(4,1000:1100));hold on; plot(envelopes(4,1040:1140));
% title('(c) Channel 4');
% axis([0 100 -inf inf]);
% subplot(2,2,4);
% plot(bandpassed1(5,1000:1100));hold on; plot(envelopes(5,1040:1140));
% title('(d) Channel 5');
% axis([0 100 -inf inf]);
%     

 
 
upEnvelopes = zeros(nFilters, length(carrier));
for i = 1: nFilters
    for j = 1: length(envelopes(i,:))
        upEnvelopes(i,(6*j)-5:(6*j)) = envelopes(i,j);
    end
end

modulated = zeros(nFilters, length(carrier));
for i = 1:nFilters
    modulated(i,:) = upEnvelopes(i,:) .* carrier;
end

bandpassed2 = zeros(nFilters, length(carrier));
for i = 1:nFilters
    bandpassed2(i,:) = filter(sFilts(i,:), 1, modulated(i,:));
%     figure();
%     plot(bandpassed2(i,1000:1500));
end

outSignal = zeros(length(carrier), 1);
for i = i:length(carrier)
    for j = 1:nFilters
        outSignal(i) = outSignal(i) + bandpassed2(j,i);
    end
end
