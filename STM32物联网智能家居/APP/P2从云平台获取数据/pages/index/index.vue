<template>
	<view class="content">
		<view class="">温度 {{temp}} ℃</view>
		<view class="">湿度 {{humi}} %</view>
		<switch :checked="led" @change="" />

	</view>
</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')

	export default {
		data() {
			return {
				temp: '',
				humi: '',
				led: true,
				token: '',
			}
		},
		onLoad() {
			const params = {
				access_key: 't/vVbWvTSz9br6mxx13BC131dc+1Nx2Tw1nz6ZWHI3U',
				version: '2022-05-01',
				productid: 'Aio2o842Wl',
			}

			this.token = createCommonToken(params);
		},
		onShow() {
			this.fetchDevData();
			setInterval(()=>{
				this.fetchDevData();
			},3000)
		},
		methods: {
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
					method: 'GET',
					data: {
						product_id: 'Aio2o842Wl',
						device_name: 'd1'
					},
					header: {
						'authorization': this.token
					},
					success: (res) => {
						console.log(res.data);
						this.temp = res.data.data[2].value;
						this.humi = res.data.data[0].value;
						this.led = res.data.data[1].value === 'true';
					}
				});

			}
		}
	}
</script>

<style>
	.content {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
	}

	.logo {
		height: 200rpx;
		width: 200rpx;
		margin-top: 200rpx;
		margin-left: auto;
		margin-right: auto;
		margin-bottom: 50rpx;
	}

	.text-area {
		display: flex;
		justify-content: center;
	}

	.title {
		font-size: 36rpx;
		color: #8f8f94;
	}
</style>