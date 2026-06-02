#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SmartIrrigate - Dashboard</title>
    <meta name="description" content="Há»‡ thá»‘ng quáº£n lÃ½ tÆ°á»›i cÃ¢y thÃ´ng minh IoT - theo dÃµi cáº£m biáº¿n, Ä‘iá»u khiá»ƒn thiáº¿t bá»‹ thá»i gian thá»±c.">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:wght,FILL@100..700,0..1&display=swap" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.min.js"></script>
    <style>
        /* ============================================
           AgroPulse Design System â€” Light Mode
           Based on Material Design 3 + Eco-Tech
           ============================================ */

        :root {
            /* Surfaces */
            --surface: #f8f9fa;
            --surface-dim: #d9dadb;
            --surface-bright: #f8f9fa;
            --surface-container-lowest: #ffffff;
            --surface-container-low: #f3f4f5;
            --surface-container: #edeeef;
            --surface-container-high: #e7e8e9;
            --surface-container-highest: #e1e3e4;

            /* On Surface */
            --on-surface: #191c1d;
            --on-surface-variant: #404943;
            --inverse-surface: #2e3132;
            --inverse-on-surface: #f0f1f2;

            /* Outline */
            --outline: #707973;
            --outline-variant: #bfc9c1;

            /* Primary â€” Leaf Green */
            --primary: #0f5238;
            --on-primary: #ffffff;
            --primary-container: #2d6a4f;
            --on-primary-container: #a8e7c5;
            --inverse-primary: #95d4b3;
            --primary-fixed: #b1f0ce;

            /* Secondary â€” Water Blue */
            --secondary: #006399;
            --on-secondary: #ffffff;
            --secondary-container: #67bafd;
            --on-secondary-container: #004972;
            --secondary-fixed: #cde5ff;

            /* Tertiary â€” Sunlight Yellow */
            --tertiary: #5f4200;
            --on-tertiary: #ffffff;
            --tertiary-container: #7e5800;
            --on-tertiary-container: #ffd388;
            --tertiary-fixed: #ffdea9;

            /* Error */
            --error: #ba1a1a;
            --on-error: #ffffff;
            --error-container: #ffdad6;
            --on-error-container: #93000a;

            /* Elevation Shadows */
            --shadow-1: 0px 4px 20px rgba(0, 0, 0, 0.04);
            --shadow-2: 0px 12px 32px rgba(0, 0, 0, 0.08);

            /* Layout */
            --sidebar-width: 280px;
            --max-content: 1440px;
            --gutter: 24px;
            --radius-sm: 0.5rem;
            --radius-md: 1rem;
            --radius-lg: 1.5rem;
            --radius-full: 9999px;
        }

        * { box-sizing: border-box; margin: 0; padding: 0; }

        body {
            font-family: 'Inter', -apple-system, sans-serif;
            background: var(--surface);
            color: var(--on-surface);
            min-height: 100vh;
            display: flex;
        }

        .material-symbols-outlined {
            font-variation-settings: 'FILL' 1;
            vertical-align: middle;
        }

        /* ============================================
           SIDEBAR NAVIGATION
           ============================================ */
        .sidebar {
            position: fixed;
            left: 0; top: 0;
            width: var(--sidebar-width);
            height: 100vh;
            background: var(--surface-container-lowest);
            border-right: 1px solid var(--outline-variant);
            box-shadow: 2px 0 8px rgba(0,0,0,0.02);
            display: flex;
            flex-direction: column;
            padding: 24px 16px;
            z-index: 20;
        }

        .sidebar-brand {
            display: flex;
            align-items: center;
            gap: 16px;
            margin-bottom: 32px;
        }

        .sidebar-logo {
            width: 48px; height: 48px;
            border-radius: var(--radius-sm);
            background: transparent;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .sidebar-logo .material-symbols-outlined {
            font-size: 28px;
            color: var(--on-primary);
        }

        .sidebar-brand-text h1 {
            font-size: 20px;
            font-weight: 600;
            line-height: 28px;
            color: var(--primary);
        }

        .sidebar-brand-text p {
            font-size: 14px;
            line-height: 20px;
            color: var(--on-surface-variant);
        }

        .sidebar-nav {
            display: flex;
            flex-direction: column;
            gap: 4px;
            flex: 1;
        }

        .nav-item {
            display: flex;
            align-items: center;
            gap: 16px;
            padding: 8px 16px;
            border-radius: var(--radius-sm);
            text-decoration: none;
            font-size: 16px;
            line-height: 24px;
            color: var(--on-surface-variant);
            transition: all 0.2s ease;
            cursor: pointer;
        }

        .nav-item:hover {
            background: var(--surface-container-high);
        }

        .nav-item.active {
            background: var(--primary);
            color: var(--on-primary);
            font-weight: 600;
            transform: scale(0.98);
        }

        .nav-item .material-symbols-outlined {
            font-size: 24px;
        }

        .sidebar-action-btn {
            width: 100%;
            padding: 10px 16px;
            border: none;
            border-radius: var(--radius-full);
            background: var(--primary);
            color: var(--on-primary);
            font-family: 'Inter', sans-serif;
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.05em;
            text-transform: uppercase;
            cursor: pointer;
            margin-top: 24px;
            transition: all 0.2s;
            box-shadow: 0 2px 6px rgba(0,0,0,0.1);
        }

        .sidebar-action-btn:hover {
            background: var(--primary-container);
            color: var(--on-primary-container);
        }

        .sidebar-footer {
            margin-top: auto;
            border-top: 1px solid var(--outline-variant);
            padding-top: 16px;
            display: flex;
            flex-direction: column;
            gap: 4px;
        }

        /* ============================================
           MAIN CONTENT
           ============================================ */
        .main-content {
            flex: 1;
            margin-left: var(--sidebar-width);
            display: flex;
            flex-direction: column;
            min-height: 100vh;
        }

        /* Top App Bar */
        .top-bar {
            position: sticky; top: 0; z-index: 10;
            background: var(--surface-bright);
            border-bottom: 1px solid var(--outline-variant);
            box-shadow: 0 1px 4px rgba(0,0,0,0.04);
        }

        .top-bar-inner {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 0 var(--gutter);
            height: 64px;
            max-width: var(--max-content);
            margin: 0 auto;
            width: 100%;
        }

        .top-bar-title {
            font-size: 24px;
            font-weight: 600;
            line-height: 32px;
            color: var(--primary);
        }

        .top-bar-actions {
            display: flex;
            align-items: center;
            gap: 16px;
        }

        .icon-btn {
            position: relative;
            width: 40px; height: 40px;
            border-radius: var(--radius-full);
            border: none;
            background: transparent;
            color: var(--on-surface-variant);
            cursor: pointer;
            display: flex;
            align-items: center;
            justify-content: center;
            transition: background 0.2s;
        }

        .icon-btn:hover {
            background: var(--surface-container-low);
        }

        .notification-dot {
            position: absolute;
            top: 6px; right: 6px;
            width: 10px; height: 10px;
            border-radius: 50%;
            background: var(--error);
            border: 2px solid var(--surface);
        }

        .avatar {
            width: 40px; height: 40px;
            border-radius: 50%;
            background: var(--surface-container-highest);
            border: 1px solid var(--outline-variant);
            cursor: pointer;
            overflow: hidden;
            transition: all 0.2s;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 16px;
            font-weight: 600;
            color: var(--primary);
        }

        .avatar:hover {
            box-shadow: 0 0 0 2px var(--primary);
        }

        /* View Sections */
        .view-section {
            display: none;
            padding: var(--gutter);
            max-width: var(--max-content);
            margin: 0 auto;
            width: 100%;
            flex: 1;
            flex-direction: column;
            gap: var(--gutter);
            animation: fadeIn 0.3s ease;
        }

        .view-section.active {
            display: flex;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(10px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* ============================================
           PREMIUM FOOTER
           ============================================ */
        .premium-footer {
            background: var(--surface-container-low);
            border-top: 1px solid var(--outline-variant);
            margin-top: auto;
            padding: 40px var(--gutter) 20px;
            color: var(--on-surface-variant);
        }
        
        .footer-grid {
            max-width: var(--max-content);
            margin: 0 auto;
            display: grid;
            grid-template-columns: 2fr 1fr 1fr;
            gap: 40px;
            margin-bottom: 40px;
        }
        
        .footer-brand p {
            font-size: 14px;
            line-height: 22px;
            max-width: 300px;
        }
        
        .footer-links h4, .footer-status h4 {
            color: var(--on-surface);
            font-size: 16px;
            font-weight: 600;
            margin-bottom: 16px;
        }
        
        .footer-links a {
            display: block;
            color: var(--on-surface-variant);
            text-decoration: none;
            font-size: 14px;
            margin-bottom: 12px;
            transition: color 0.2s;
        }
        
        .footer-links a:hover {
            color: var(--primary);
        }
        
        .footer-status {
            font-size: 14px;
        }
        
        .status-dot {
            width: 8px; height: 8px;
            border-radius: 50%;
            display: inline-block;
        }
        
        .status-dot.online { background: var(--primary); box-shadow: 0 0 8px var(--primary); }
        .status-dot.offline { background: var(--error); box-shadow: 0 0 8px var(--error); }
        
        .footer-bottom {
            max-width: var(--max-content);
            margin: 0 auto;
            padding-top: 20px;
            border-top: 1px solid var(--outline-variant);
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-size: 12px;
        }
        
        .footer-bottom a {
            color: var(--on-surface-variant);
            text-decoration: none;
        }
        
        .footer-bottom a:hover { color: var(--primary); }
        
        @media (max-width: 768px) {
            .footer-grid { grid-template-columns: 1fr; gap: 24px; }
            .footer-bottom { flex-direction: column; gap: 12px; text-align: center; }
        }

        /* ============================================
           CARDS
           ============================================ */
        .card {
            background: var(--surface-container-lowest);
            border-radius: var(--radius-md);
            padding: 24px;
            box-shadow: var(--shadow-1);
            border: 1px solid var(--outline-variant);
            transition: box-shadow 0.3s ease;
        }

        .card:hover {
            box-shadow: var(--shadow-2);
        }

        .card-header {
            display: flex;
            justify-content: space-between;
            align-items: flex-start;
            margin-bottom: 16px;
        }

        .card-title-group {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .card-title-group .material-symbols-outlined {
            color: var(--on-surface-variant);
        }

        .card-title {
            font-size: 20px;
            font-weight: 600;
            line-height: 28px;
            color: var(--on-surface);
        }

        .card-menu-btn {
            background: none;
            border: none;
            color: var(--outline);
            cursor: pointer;
            padding: 4px;
            border-radius: var(--radius-full);
        }

        .card-menu-btn:hover {
            background: var(--surface-container-high);
        }

        /* Hero Card â€” Soil Moisture */
        .hero-card {
            display: flex;
            align-items: center;
            justify-content: space-between;
            flex-wrap: wrap;
            gap: 24px;
        }

        .hero-left {
            display: flex;
            align-items: center;
            gap: 24px;
        }

        .hero-icon {
            width: 64px; height: 64px;
            border-radius: 50%;
            background: var(--secondary-container);
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .hero-icon .material-symbols-outlined {
            font-size: 36px;
            color: var(--on-secondary-container);
        }

        .hero-value-group h3 {
            font-size: 20px;
            font-weight: 600;
            color: var(--on-surface);
            margin-bottom: 4px;
        }

        .hero-value {
            font-size: 48px;
            font-weight: 700;
            line-height: 56px;
            letter-spacing: -0.02em;
            color: var(--primary);
            font-variant-numeric: tabular-nums;
        }

        .status-badge {
            display: inline-flex;
            align-items: center;
            padding: 4px 12px;
            border-radius: var(--radius-full);
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.05em;
            margin-left: 8px;
            vertical-align: middle;
        }

        .status-badge.standby {
            background: var(--surface-container);
            color: var(--on-surface-variant);
        }

        .status-badge.active {
            background: var(--primary-fixed);
            color: var(--primary);
        }

        .status-badge.error {
            background: var(--error-container);
            color: var(--on-error-container);
        }

        /* Toggle Switch â€” Material Design */
        .toggle-group {
            display: flex;
            align-items: center;
            gap: 12px;
            padding: 8px 16px;
            background: var(--surface);
            border-radius: var(--radius-sm);
            border: 1px solid var(--outline-variant);
        }

        .toggle-label {
            font-size: 16px;
            line-height: 24px;
            color: var(--on-surface-variant);
        }

        .md-switch {
            position: relative;
            width: 44px; height: 24px;
            cursor: pointer;
            display: inline-block;
        }

        .md-switch input { opacity: 0; width: 0; height: 0; position: absolute; }

        .md-switch .track {
            position: absolute; inset: 0;
            background: var(--surface-container-highest);
            border-radius: var(--radius-full);
            transition: all 0.25s cubic-bezier(0.4, 0, 0.2, 1);
        }

        .md-switch .track::before {
            content: '';
            position: absolute;
            left: 2px; top: 2px;
            width: 20px; height: 20px;
            background: white;
            border-radius: 50%;
            border: 1px solid rgba(0,0,0,0.08);
            box-shadow: 0 2px 4px rgba(0,0,0,0.12);
            transition: all 0.25s cubic-bezier(0.4, 0, 0.2, 1);
        }

        .md-switch input:checked + .track {
            background: var(--primary);
        }

        .md-switch input:checked + .track::before {
            transform: translateX(20px);
        }

        /* Grid Section */
        .grid-2 {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: var(--gutter);
        }

        /* Climate Card */
        .climate-values {
            display: flex;
            justify-content: space-between;
            align-items: flex-end;
            margin-bottom: 24px;
        }

        .climate-item label {
            font-size: 14px;
            line-height: 20px;
            color: var(--on-surface-variant);
            margin-bottom: 4px;
            display: block;
        }

        .climate-item .value {
            font-size: 32px;
            font-weight: 600;
            line-height: 40px;
            letter-spacing: -0.01em;
            color: var(--on-surface);
            font-variant-numeric: tabular-nums;
        }

        .climate-item.right {
            text-align: right;
        }

        .alert-chip {
            display: inline-flex;
            align-items: center;
            gap: 6px;
            padding: 6px 12px;
            border-radius: var(--radius-full);
            font-size: 12px;
            font-weight: 600;
            letter-spacing: 0.05em;
        }

        .alert-chip.warning {
            background: var(--error-container);
            color: var(--on-error-container);
        }

        .alert-chip.info {
            background: var(--primary-fixed);
            color: var(--primary);
        }

        .alert-chip.caution {
            background: var(--tertiary-fixed);
            color: var(--tertiary);
        }

        .alert-chip .material-symbols-outlined {
            font-size: 16px;
        }

        /* Lighting Card */
        .light-value {
            margin-bottom: 24px;
        }

        .light-value label {
            font-size: 14px;
            line-height: 20px;
            color: var(--on-surface-variant);
            margin-bottom: 4px;
            display: block;
        }

        .light-value .value {
            font-size: 32px;
            font-weight: 600;
            line-height: 40px;
            color: var(--on-surface);
            font-variant-numeric: tabular-nums;
        }

        .light-value .unit {
            font-size: 16px;
            font-weight: 400;
            color: var(--outline);
        }

        .grow-led-row {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 8px 16px;
            background: var(--surface);
            border-radius: var(--radius-sm);
            border: 1px solid var(--outline-variant);
        }

        .grow-led-left {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .grow-led-left .material-symbols-outlined {
            color: var(--primary);
        }

        .grow-led-left span.label {
            font-size: 16px;
            color: var(--on-surface-variant);
        }

        .auto-badge {
            display: inline-flex;
            padding: 2px 8px;
            border-radius: var(--radius-full);
            background: var(--primary-container);
            color: var(--on-primary-container);
            font-size: 10px;
            font-weight: 600;
            letter-spacing: 0.05em;
        }

        /* Water Tank Card â€” Error State */
        .tank-card {
            background: var(--error-container);
            border-color: rgba(186, 26, 26, 0.3);
        }

        .tank-card.normal {
            background: var(--surface-container-lowest);
            border-color: var(--outline-variant);
        }

        .tank-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 16px;
        }

        .tank-title-group {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .tank-title-group .material-symbols-outlined {
            color: var(--error);
        }

        .tank-card.normal .tank-title-group .material-symbols-outlined {
            color: var(--secondary);
        }

        .tank-title {
            font-size: 20px;
            font-weight: 600;
            line-height: 28px;
            color: var(--on-error-container);
        }

        .tank-card.normal .tank-title {
            color: var(--on-surface);
        }

        .tank-level {
            font-size: 24px;
            font-weight: 600;
            line-height: 32px;
            font-variant-numeric: tabular-nums;
        }

        .tank-level.error { color: var(--error); }
        .tank-level.warning { color: var(--tertiary); }
        .tank-level.ok { color: var(--primary); }

        /* Progress Track */
        .progress-bar {
            width: 100%;
            height: 8px;
            background: var(--surface-container-highest);
            border-radius: var(--radius-full);
            overflow: hidden;
            margin-bottom: 16px;
        }

        .progress-bar .fill {
            height: 100%;
            border-radius: var(--radius-full);
            transition: width 0.5s cubic-bezier(0.4, 0, 0.2, 1);
            background: var(--secondary);
        }

        .progress-bar .fill.error { background: var(--error); }
        .progress-bar .fill.warning { background: var(--tertiary); }

        .tank-warning {
            display: flex;
            align-items: center;
            gap: 8px;
            padding: 8px 16px;
            border-radius: var(--radius-sm);
            background: rgba(255,255,255,0.5);
            font-size: 14px;
            line-height: 20px;
            color: var(--error);
        }

        .tank-warning .material-symbols-outlined {
            font-size: 20px;
        }

        .tank-card.normal .tank-warning {
            background: var(--surface-container-low);
            color: var(--on-surface-variant);
        }

        /* ============================================
           SIMULATION PANEL
           ============================================ */
        .sim-panel {
            background: var(--surface-container-lowest);
            border-radius: var(--radius-md);
            padding: 24px;
            box-shadow: var(--shadow-1);
            border: 1px solid var(--outline-variant);
        }

        .sim-header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 20px;
            cursor: pointer;
        }

        .sim-title-group {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .sim-title {
            font-size: 20px;
            font-weight: 600;
            color: var(--on-surface);
        }

        .sim-title-group .material-symbols-outlined {
            color: var(--tertiary);
        }

        .sim-badge {
            font-size: 10px;
            font-weight: 600;
            letter-spacing: 0.05em;
            padding: 4px 10px;
            border-radius: var(--radius-full);
            background: var(--tertiary-fixed);
            color: var(--tertiary);
        }

        .sim-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
        }

        .sim-slider-group {
            display: flex;
            flex-direction: column;
            gap: 8px;
        }

        .sim-slider-group.full-width {
            grid-column: 1 / -1;
        }

        .sim-slider-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .sim-slider-label {
            font-size: 14px;
            font-weight: 500;
            color: var(--on-surface-variant);
        }

        .sim-slider-val {
            font-size: 14px;
            font-weight: 700;
            font-variant-numeric: tabular-nums;
            color: var(--on-surface);
        }

        /* Range Input â€” Material Style */
        input[type=range] {
            width: 100%;
            appearance: none;
            background: transparent;
            cursor: pointer;
            height: 24px;
        }

        input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 4px;
            background: var(--surface-container-highest);
            border-radius: var(--radius-full);
        }

        input[type=range]::-webkit-slider-thumb {
            appearance: none;
            width: 20px; height: 20px;
            border-radius: 50%;
            background: var(--primary);
            margin-top: -8px;
            box-shadow: 0 2px 6px rgba(0,0,0,0.15);
            transition: transform 0.15s;
        }

        input[type=range]::-webkit-slider-thumb:hover {
            transform: scale(1.15);
            box-shadow: 0 0 0 8px rgba(15, 82, 56, 0.08);
        }

        input[type=range].secondary::-webkit-slider-thumb { background: var(--secondary); }
        input[type=range].tertiary::-webkit-slider-thumb { background: var(--tertiary-container); }
        input[type=range].error::-webkit-slider-thumb { background: var(--error); }

        /* ============================================
           MODE BAR
           ============================================ */
        .mode-bar {
            display: flex;
            align-items: center;
            gap: 8px;
            padding: 6px 12px;
            border-radius: var(--radius-full);
            background: var(--surface-container);
            font-size: 14px;
            color: var(--on-surface-variant);
        }

        .mode-bar .mode-text {
            font-weight: 600;
            color: var(--primary);
        }

        /* ============================================
           RESPONSIVE
           ============================================ */
        @media (max-width: 900px) {
            .sidebar { display: none; }
            .main-content { margin-left: 0; }
            .grid-2 { grid-template-columns: 1fr; }
            .sim-grid { grid-template-columns: 1fr; }
            .hero-card { flex-direction: column; align-items: flex-start; }
            .mobile-menu-btn { display: block !important; }
        }

        .mobile-menu-btn { display: none; }
    </style>
</head>
<body>

    <!-- ============ SIDEBAR ============ -->
    <nav class="sidebar" id="sidebar">
        <div class="sidebar-brand">
            <div class="sidebar-logo">
                <img src="./logo1.png" alt="Logo" style="width: 100%; height: 100%; object-fit: cover;">
            </div>
            <div class="sidebar-brand-text">
                <h1>SmartIrrigate</h1>
                <p id="sidebar-status">Äang hoáº¡t Ä‘á»™ng</p>
            </div>
        </div>

        <div class="sidebar-nav">
            <a class="nav-item active" href="#" data-target="view-dashboard">
                <span class="material-symbols-outlined">dashboard</span>
                <span>Báº£ng Ä‘iá»u khiá»ƒn</span>
            </a>
            <a class="nav-item" href="#" data-target="view-sensors">
                <span class="material-symbols-outlined">sensors</span>
                <span>Cáº£m biáº¿n</span>
            </a>
            <a class="nav-item" href="#" data-target="view-controls">
                <span class="material-symbols-outlined">settings_remote</span>
                <span>Äiá»u khiá»ƒn</span>
            </a>
            <a class="nav-item" href="#" data-target="view-schedules">
                <span class="material-symbols-outlined">calendar_today</span>
                <span>Lá»‹ch trÃ¬nh</span>
            </a>
            <a class="nav-item" href="#" data-target="view-analytics">
                <span class="material-symbols-outlined">bar_chart</span>
                <span>Thá»‘ng kÃª</span>
            </a>
            <a class="nav-item" href="#" data-target="view-logs">
                <span class="material-symbols-outlined">receipt_long</span>
                <span>Nháº­t kÃ½</span>
            </a>
            <a class="nav-item" href="#" data-target="view-settings">
                <span class="material-symbols-outlined">settings_suggest</span>
                <span>CÃ i Ä‘áº·t</span>
            </a>

            <button class="sidebar-action-btn" id="btn-harvest">Thu hoáº¡ch nhanh</button>
        </div>

        <div class="sidebar-footer">
            <a class="nav-item" href="#">
                <span class="material-symbols-outlined">help</span>
                <span>Há»— trá»£</span>
            </a>
            <a class="nav-item" href="#">
                <span class="material-symbols-outlined">settings</span>
                <span>CÃ i Ä‘áº·t</span>
            </a>
        </div>
    </nav>

    <!-- ============ MAIN ============ -->
    <main class="main-content">
        <!-- Top App Bar -->
        <header class="top-bar">
            <div class="top-bar-inner">
                <div style="display:flex;align-items:center;gap:16px;">
                    <button class="icon-btn mobile-menu-btn" id="menu-btn">
                        <span class="material-symbols-outlined">menu</span>
                    </button>
                    <h2 class="top-bar-title">Khu vÆ°á»n cá»§a tÃ´i</h2>
                </div>
                <div class="top-bar-actions">
                    <div class="mode-bar">
                        <span class="material-symbols-outlined" style="font-size:18px;">smart_toy</span>
                        <span id="mode-label" class="mode-text">Tá»± Ä‘á»™ng</span>
                        <label class="md-switch">
                            <input type="checkbox" id="mode-switch" checked>
                            <span class="track"></span>
                        </label>
                    </div>
                    <button class="icon-btn" id="notif-btn">
                        <span class="material-symbols-outlined">notifications</span>
                        <span class="notification-dot" id="notif-dot"></span>
                    </button>
                    <div class="avatar">KT</div>
                </div>
            </div>
        </header>

        <!-- Dashboard Canvas -->
        <div class="view-section active" id="view-dashboard">

            <!-- ===== HERO: Soil Moisture ===== -->
            <div class="card hero-card" id="card-soil">
                <div class="hero-left">
                    <div class="hero-icon">
                        <span class="material-symbols-outlined">water_drop</span>
                    </div>
                    <div class="hero-value-group">
                        <h3>Äá»™ áº©m Äáº¥t</h3>
                        <div>
                            <span class="hero-value" id="v-soil">50</span><span class="hero-value" style="font-size:32px">%</span>
                            <span class="status-badge standby" id="soil-badge">Cháº¿ Ä‘á»™ chá»</span>
                        </div>
                    </div>
                </div>
                <div class="toggle-group">
                    <span class="toggle-label">Báº­t/táº¯t MÃ¡y bÆ¡m</span>
                    <label class="md-switch">
                        <input type="checkbox" id="sw-pump">
                        <span class="track"></span>
                    </label>
                </div>
            </div>

            <!-- ===== GRID: Climate + Lighting ===== -->
            <div class="grid-2">
                <!-- Climate -->
                <div class="card" id="card-climate">
                    <div class="card-header">
                        <div class="card-title-group">
                            <span class="material-symbols-outlined">thermostat</span>
                            <h4 class="card-title">KhÃ­ háº­u (DHT22)</h4>
                        </div>
                        <button class="card-menu-btn"><span class="material-symbols-outlined">more_vert</span></button>
                    </div>
                    <div class="climate-values">
                        <div class="climate-item">
                            <label>Nhiá»‡t Ä‘á»™</label>
                            <span class="value" id="v-temp">25Â°C</span>
                        </div>
                        <div class="climate-item right">
                            <label>Äá»™ áº©m</label>
                            <span class="value" id="v-hum">60%</span>
                        </div>
                    </div>
                    <div id="climate-chip"></div>
                </div>

                <!-- Lighting -->
                <div class="card" id="card-light">
                    <div class="card-header">
                        <div class="card-title-group">
                            <span class="material-symbols-outlined">light_mode</span>
                            <h4 class="card-title">Ãnh sÃ¡ng (LDR)</h4>
                        </div>
                        <button class="card-menu-btn"><span class="material-symbols-outlined">more_vert</span></button>
                    </div>
                    <div class="light-value">
                        <label>CÆ°á»ng Ä‘á»™ sÃ¡ng</label>
                        <span class="value" id="v-light">500</span> <span class="unit">Lux</span>
                    </div>
                    <div class="grow-led-row">
                        <div class="grow-led-left">
                            <span class="material-symbols-outlined">emoji_objects</span>
                            <span class="label">ÄÃ¨n quang há»£p</span>
                            <span class="auto-badge" id="led-auto-badge">Tá»± Ä‘á»™ng</span>
                        </div>
                        <label class="md-switch">
                            <input type="checkbox" id="sw-led">
                            <span class="track"></span>
                        </label>
                    </div>
                    <div class="grow-led-row" style="margin-top: 12px; border-top: 1px dashed var(--outline-variant); padding-top: 12px;">
                        <div class="grow-led-left">
                            <span class="material-symbols-outlined" id="fan-icon">mode_fan</span>
                            <span class="label">Quáº¡t thÃ´ng giÃ³</span>
                            <span class="auto-badge" id="fan-auto-badge">Tá»± Ä‘á»™ng</span>
                        </div>
                        <label class="md-switch">
                            <input type="checkbox" id="sw-fan">
                            <span class="track"></span>
                        </label>
                    </div>
                </div>
            </div>

            <!-- ===== Water Tank ===== -->
            <div class="card tank-card" id="card-tank">
                <div class="tank-header">
                    <div class="tank-title-group">
                        <span class="material-symbols-outlined" id="tank-icon">water_lock</span>
                        <h4 class="tank-title" id="tank-title-text">Má»±c nÆ°á»›c Bá»ƒ chá»©a</h4>
                    </div>
                    <span class="tank-level error" id="v-water-display">80%</span>
                </div>
                <div class="progress-bar">
                    <div class="fill" id="bar-water" style="width:80%"></div>
                </div>
                <div class="tank-warning" id="tank-msg">
                    <span class="material-symbols-outlined">check_circle</span>
                    <span>Má»±c nÆ°á»›c Ä‘áº§y Ä‘á»§. Há»‡ thá»‘ng hoáº¡t Ä‘á»™ng bÃ¬nh thÆ°á»ng.</span>
                </div>
            </div>

        </div>
        
        <!-- Additional Views Placeholders -->
        <div class="view-section" id="view-sensors">
            <h2 style="margin-bottom: 16px; color: var(--primary);">Dá»¯ liá»‡u Cáº£m biáº¿n Chi tiáº¿t</h2>
            <div class="sim-grid">
                <div class="card" style="text-align: center;">
                    <h4 style="color: var(--on-surface-variant); margin-bottom: 8px;">Nhiá»‡t Ä‘á»™ MÃ´i trÆ°á»ng</h4>
                    <p style="font-size: 32px; font-weight: bold; color: var(--on-surface);" id="v-temp-sensor">--Â°C</p>
                </div>
                <div class="card" style="text-align: center;">
                    <h4 style="color: var(--on-surface-variant); margin-bottom: 8px;">Äá»™ áº©m KhÃ´ng khÃ­</h4>
                    <p style="font-size: 32px; font-weight: bold; color: var(--on-surface);" id="v-hum-sensor">--%</p>
                </div>
                <div class="card" style="text-align: center;">
                    <h4 style="color: var(--on-surface-variant); margin-bottom: 8px;">Äá»™ áº©m Äáº¥t</h4>
                    <p style="font-size: 32px; font-weight: bold; color: var(--on-surface);" id="v-soil-sensor">--%</p>
                </div>
                <div class="card" style="text-align: center;">
                    <h4 style="color: var(--on-surface-variant); margin-bottom: 8px;">CÆ°á»ng Ä‘á»™ Ãnh sÃ¡ng</h4>
                    <p style="font-size: 32px; font-weight: bold; color: var(--on-surface);" id="v-light-sensor">-- Lux</p>
                </div>
                <div class="card" style="text-align: center; grid-column: 1 / -1;">
                    <h4 style="color: var(--on-surface-variant); margin-bottom: 8px;">Má»±c nÆ°á»›c</h4>
                    <p style="font-size: 32px; font-weight: bold; color: var(--secondary);" id="v-tank-sensor">--%</p>
                </div>
            </div>
        </div>

        <div class="view-section" id="view-controls">
            <h2 style="margin-bottom: 16px; color: var(--primary);">Tráº¡ng thÃ¡i Thiáº¿t bá»‹</h2>
            <div class="card" style="padding: 0;">
                <div style="display:flex; justify-content: space-between; padding: 20px; border-bottom: 1px solid var(--outline-variant);">
                    <span style="font-weight: 600; color: var(--on-surface-variant);">Cháº¿ Ä‘á»™ há»‡ thá»‘ng</span>
                    <span id="mode-status" style="color: var(--primary); font-weight: bold;">--</span>
                </div>
                <div style="display:flex; justify-content: space-between; padding: 20px; border-bottom: 1px solid var(--outline-variant);">
                    <span style="font-weight: 600; color: var(--on-surface-variant);">MÃ¡y bÆ¡m nÆ°á»›c</span>
                    <span id="pump-status" style="color: var(--primary); font-weight: bold;">--</span>
                </div>
                <div style="display:flex; justify-content: space-between; padding: 20px; border-bottom: 1px solid var(--outline-variant);">
                    <span style="font-weight: 600; color: var(--on-surface-variant);">ÄÃ¨n quang há»£p</span>
                    <span id="led-status" style="color: var(--primary); font-weight: bold;">--</span>
                </div>
                <div style="display:flex; justify-content: space-between; padding: 20px;">
                    <span style="font-weight: 600; color: var(--on-surface-variant);">Quáº¡t thÃ´ng giÃ³ (Servo)</span>
                    <span id="fan-status" style="color: var(--primary); font-weight: bold;">--</span>
                </div>
            </div>
        </div>

        <div class="view-section" id="view-schedules">
            <h2 style="margin-bottom: 16px; color: var(--primary);">Lá»‹ch trÃ¬nh TÆ°á»›i</h2>
            <div class="card">
                <p style="color: var(--on-surface-variant); margin-bottom: 16px;">Há»‡ thá»‘ng Ä‘ang hoáº¡t Ä‘á»™ng á»Ÿ cháº¿ Ä‘á»™ <b id="schedule-mode">--</b>. Lá»‹ch trÃ¬nh sáº½ tá»± Ä‘á»™ng Ä‘iá»u chá»‰nh theo dá»¯ liá»‡u cáº£m biáº¿n.</p>
                <div style="padding: 16px; background: var(--surface-container-low); border-radius: var(--radius-sm);">
                    <span class="material-symbols-outlined" style="color: var(--primary); vertical-align: bottom;">schedule</span>
                    Chu ká»³ kiá»ƒm tra Ä‘á»™ áº©m: <b>Má»—i 1 giÃ¢y</b>
                </div>
            </div>
        </div>

        <div class="view-section" id="view-analytics">
            <h2 style="margin-bottom: 16px; color: var(--primary);">Thá»‘ng kÃª Nhanh & Lá»‹ch sá»­</h2>
            <div class="grid-2" style="margin-bottom: 24px;">
                <div class="card" style="text-align: center;">
                    <span class="material-symbols-outlined" style="font-size: 40px; color: var(--secondary); margin-bottom: 12px;">water_drop</span>
                    <h4 style="margin: 0 0 8px 0; color: var(--on-surface-variant);">NÆ°á»›c Ä‘Ã£ dÃ¹ng (Æ°á»›c tÃ­nh)</h4>
                    <p style="font-size: 24px; font-weight: bold; color: var(--on-surface);">12.5 L</p>
                </div>
                <div class="card" style="text-align: center;">
                    <span class="material-symbols-outlined" style="font-size: 40px; color: var(--tertiary); margin-bottom: 12px;">wb_sunny</span>
                    <h4 style="margin: 0 0 8px 0; color: var(--on-surface-variant);">Thá»i gian chiáº¿u sÃ¡ng</h4>
                    <p style="font-size: 24px; font-weight: bold; color: var(--on-surface);">4h 30m</p>
                </div>
            </div>
            
            <div class="card" style="margin-bottom: 24px;">
                <h3 style="margin-bottom: 16px; color: var(--primary);">Biá»ƒu Ä‘á»“ Nhiá»‡t Ä‘á»™ & Äá»™ áº©m Ä‘áº¥t (100 Ä‘iá»ƒm dá»¯ liá»‡u)</h3>
                <canvas id="historyChart" width="100%" height="30"></canvas>
            </div>
        </div>
        
        <!-- View Logs -->
        <div class="view-section" id="view-logs">
            <h2 style="margin-bottom: 24px; color: var(--primary);">Nháº­t kÃ½ hoáº¡t Ä‘á»™ng</h2>
            <div class="card" style="min-height: 400px; max-height: 500px; overflow-y: auto;" id="logs-container">
                <div style="padding: 12px; border-bottom: 1px solid var(--outline-variant); color: var(--on-surface-variant);">
                    Há»‡ thá»‘ng Ä‘Ã£ sáºµn sÃ ng ghi nháº­n lá»‹ch sá»­...
                </div>
            </div>
        </div>

        <!-- View Settings -->
        <div class="view-section" id="view-settings">
            <h2 style="margin-bottom: 24px; color: var(--primary);">CÃ i Ä‘áº·t thÃ´ng sá»‘</h2>
            <div class="card">
                <h3 style="margin-bottom: 16px; color: var(--primary);">Cáº¥u hÃ¬nh NgÆ°á»¡ng (Thresholds)</h3>
                
                <div class="sim-grid">
                    <div class="sim-slider-group">
                        <div class="sim-slider-header">
                            <span class="sim-slider-label">NgÆ°á»¡ng Ä‘á»™ áº©m Ä‘áº¥t (cáº§n tÆ°á»›i)</span>
                            <span class="sim-slider-val" id="val-set-soil">40%</span>
                        </div>
                        <input type="range" id="set-soil" min="0" max="100" value="40">
                        <button class="sidebar-action-btn" style="margin-top: 12px; width: auto;" onclick="saveSetting('soil')">LÆ°u NgÆ°á»¡ng Äáº¥t</button>
                    </div>

                    <div class="sim-slider-group">
                        <div class="sim-slider-header">
                            <span class="sim-slider-label">NgÆ°á»¡ng nhiá»‡t Ä‘á»™ (cáº£nh bÃ¡o)</span>
                            <span class="sim-slider-val" id="val-set-temp">35Â°C</span>
                        </div>
                        <input type="range" class="error" id="set-temp" min="20" max="60" value="35" step="0.5">
                        <button class="sidebar-action-btn" style="background: var(--error); margin-top: 12px; width: auto;" onclick="saveSetting('temp')">LÆ°u NgÆ°á»¡ng Nhiá»‡t Ä‘á»™</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- Premium Footer -->
        <footer class="premium-footer">
            <div class="footer-grid">
                <div class="footer-brand">
                    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 12px;">
                        <span class="material-symbols-outlined" style="color: var(--primary);">eco</span>
                        <h3 style="color: var(--primary); font-weight: 700;">SmartIrrigate</h3>
                    </div>
                    <p>Há»‡ thá»‘ng giÃ¡m sÃ¡t vÃ  tÆ°á»›i tiÃªu tá»± Ä‘á»™ng IoT, giÃºp tá»‘i Æ°u hÃ³a lÆ°á»£ng nÆ°á»›c vÃ  nÄƒng suáº¥t cÃ¢y trá»“ng cá»§a báº¡n má»—i ngÃ y.</p>
                </div>
                <div class="footer-links">
                    <h4>Quáº£n lÃ½</h4>
                    <a href="#" onclick="document.querySelector('[data-target=\'view-dashboard\']').click()">Báº£ng Ä‘iá»u khiá»ƒn</a>
                    <a href="#" onclick="document.querySelector('[data-target=\'view-schedules\']').click()">Lá»‹ch sá»­ & LÃªn lá»‹ch</a>
                    <a href="#" onclick="document.querySelector('[data-target=\'view-controls\']').click()">CÃ i Ä‘áº·t thiáº¿t bá»‹</a>
                </div>
                <div class="footer-status">
                    <h4>Tráº¡ng thÃ¡i Há»‡ thá»‘ng</h4>
                    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 8px;">
                        <span class="status-dot online"></span> MÃ¡y chá»§ Web: á»”n Ä‘á»‹nh
                    </div>
                    <div style="display: flex; align-items: center; gap: 8px;">
                        <span class="status-dot offline" id="footer-ws-dot"></span> WebSocket: <span id="footer-ws-text">Äang ngáº¯t káº¿t ná»‘i</span>
                    </div>
                </div>
            </div>
            <div class="footer-bottom">
                <p>&copy; 2026 AgroPulse by GreenHome. All rights reserved.</p>
                <div style="display:flex; gap: 16px;">
                    <a href="#">Quyá»n riÃªng tÆ°</a>
                    <a href="#">Äiá»u khoáº£n sá»­ dá»¥ng</a>
                </div>
            </div>
        </footer>
    </main>

    <script>
        // ========== NAVIGATION LOGIC ==========
        const navItems = document.querySelectorAll('.nav-item[data-target]');
        const viewSections = document.querySelectorAll('.view-section');
        
        navItems.forEach(item => {
            item.addEventListener('click', (e) => {
                e.preventDefault();
                navItems.forEach(nav => nav.classList.remove('active'));
                item.classList.add('active');
                
                const targetId = item.getAttribute('data-target');
                viewSections.forEach(view => {
                    view.classList.remove('active');
                });
                const targetView = document.getElementById(targetId);
                if(targetView) targetView.classList.add('active');
            });
        });

        // ========== DOM ==========
        const $ = id => document.getElementById(id);

        const modeSwitch = $('mode-switch');
        const swPump = $('sw-pump');
        const swLed = $('sw-led');
        const swFan = $('sw-fan'); // ThÃªm swFan

        let isPumpOn = false;
        let isLedOn = false;
        let isFanOn = false;
        let isAutoMode = true;

        // Start with auto â†’ switches are read-only
        swPump.style.pointerEvents = 'none';
        swLed.style.pointerEvents = 'none';
        swFan.style.pointerEvents = 'none';

        // ========== MQTT ==========
        const mqtt_server = "broker.hivemq.com";
        const mqtt_port = 8000;
        const topic_state = "smartgarden_v1/state";
        const topic_cmd   = "smartgarden_v1/cmd";

        const clientId = "WebClient-" + Math.random().toString(16).substr(2, 8);
        const mqttClient = new Paho.MQTT.Client(mqtt_server, mqtt_port, clientId);

        mqttClient.onConnectionLost = onConnectionLost;
        mqttClient.onMessageArrived = onMessageArrived;

        mqttClient.connect({
            onSuccess: onConnect,
            onFailure: function (e) {
                console.log(e);
                $('sidebar-status').textContent = 'Disconnected';
                $('sidebar-status').style.color = 'var(--error)';
                if($('footer-ws-dot')) {
                    $('footer-ws-dot').className = 'status-dot offline';
                    $('footer-ws-text').textContent = 'Lá»—i káº¿t ná»‘i MQTT';
                }
            }
        });

        function onConnect() {
            console.log("MQTT Connected");
            $('sidebar-status').textContent = 'Connected (MQTT)';
            $('sidebar-status').style.color = 'var(--primary)';
            if($('footer-ws-dot')) {
                $('footer-ws-dot').className = 'status-dot online';
                $('footer-ws-text').textContent = 'Káº¿t ná»‘i qua Cloud (Chuáº©n IoT)';
            }
            mqttClient.subscribe(topic_state);
        }

        function onConnectionLost(responseObject) {
            if (responseObject.errorCode !== 0) {
                console.log("onConnectionLost:" + responseObject.errorMessage);
            }
            $('sidebar-status').textContent = 'Disconnected';
            $('sidebar-status').style.color = 'var(--error)';
            if($('footer-ws-dot')) {
                $('footer-ws-dot').className = 'status-dot offline';
                $('footer-ws-text').textContent = 'Máº¥t káº¿t ná»‘i MQTT';
            }
        }

        function onMessageArrived(message) {
            try {
                if (message.destinationName === topic_state) {
                    const data = JSON.parse(message.payloadString);
                    if(data.type === 'state') {
                        updateUI(data);
                    }
                }
            } catch (e) {
                console.error(e);
            }
        }

        function sendCommand(cmd, value) {
            if (mqttClient.isConnected()) {
                const payload = JSON.stringify({ cmd: cmd, value: value });
                const message = new Paho.MQTT.Message(payload);
                message.destinationName = topic_cmd;
                mqttClient.send(message);
            }
        }

        // ========== MODE & CONTROL ==========
        modeSwitch.addEventListener('change', () => {
            isAutoMode = modeSwitch.checked;
            $('mode-label').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('led-auto-badge').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('led-auto-badge').style.background = isAutoMode
                ? 'var(--primary-container)' : 'var(--tertiary-fixed)';
            $('led-auto-badge').style.color = isAutoMode
                ? 'var(--on-primary-container)' : 'var(--tertiary)';

            $('fan-auto-badge').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('fan-auto-badge').style.background = isAutoMode
                ? 'var(--primary-container)' : 'var(--tertiary-fixed)';
            $('fan-auto-badge').style.color = isAutoMode
                ? 'var(--on-primary-container)' : 'var(--tertiary)';

            if (!isAutoMode) {
                swPump.style.pointerEvents = 'auto';
                swLed.style.pointerEvents = 'auto';
                swFan.style.pointerEvents = 'auto';
            } else {
                swPump.style.pointerEvents = 'none';
                swLed.style.pointerEvents = 'none';
                swFan.style.pointerEvents = 'none';
            }
            sendCommand('auto', isAutoMode);
        });

        swPump.addEventListener('change', () => { if (!isAutoMode) sendCommand('pump', swPump.checked); });
        swLed.addEventListener('change', () => { if (!isAutoMode) sendCommand('led', swLed.checked); });
        swFan.addEventListener('change', () => { if (!isAutoMode) sendCommand('fan', swFan.checked); });

        // ========== SETTINGS & LOGS ==========
        function saveSetting(type) {
            if (type === 'soil') {
                const val = parseInt($('set-soil').value);
                sendCommand("set_threshold_soil", val);
                addLog("User thay Ä‘á»•i ngÆ°á»¡ng áº©m Ä‘áº¥t thÃ nh " + val + "%", "info");
            } else if (type === 'temp') {
                const val = parseFloat($('set-temp').value);
                sendCommand("set_threshold_temp", val);
                addLog("User thay Ä‘á»•i ngÆ°á»¡ng bÃ¡o nhiá»‡t thÃ nh " + val + "Â°C", "warning");
            }
        }

        $('set-soil').addEventListener('input', (e) => {
            $('val-set-soil').textContent = e.target.value + "%";
        });
        $('set-temp').addEventListener('input', (e) => {
            $('val-set-temp').textContent = e.target.value + "Â°C";
        });

        function addLog(message, type="info") {
            const container = $('logs-container');
            const d = new Date();
            const timeStr = d.toLocaleTimeString();
            let color = "var(--on-surface-variant)";
            if(type === "warning") color = "var(--error)";
            if(type === "success") color = "var(--primary)";

            const item = document.createElement('div');
            item.style = `padding: 12px; border-bottom: 1px solid var(--outline-variant); color: ${color}; font-weight: 500;`;
            item.innerHTML = `[${timeStr}] ${message}`;
            container.prepend(item);
        }

        // ========== UPDATE UI ==========
        let prevPump = false;
        let prevWaterWarning = false;
        let prevTempWarning = false;
        let isFirstUpdate = true;

        function updateUI(data) {
            const soil = data.soil;
            const water = data.water;
            const temp = data.temp;
            const hum = data.hum;
            const light = data.light;
            isPumpOn = data.pump;
            isLedOn = data.led;
            isFanOn = data.fan;
            isAutoMode = data.auto;
            
            // Sync switches if they were changed externally
            modeSwitch.checked = isAutoMode;
            swPump.checked = isPumpOn;
            swLed.checked = isLedOn;
            swFan.checked = isFanOn;

            // --- Event Logging ---
            if(!isFirstUpdate) {
                if(data.pump !== prevPump) {
                    if(data.pump) addLog(`BÆ¡m ÄÆ¯á»¢C Báº¬T (${isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng'})`, "success");
                    else addLog(`BÆ¡m ÄÃƒ Táº®T (${isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng'})`, "info");
                }
                if(data.water <= 10 && !prevWaterWarning) {
                    addLog("Cáº¢NH BÃO: Má»©c nÆ°á»›c quÃ¡ tháº¥p!", "warning");
                    prevWaterWarning = true;
                } else if (data.water > 10) {
                    prevWaterWarning = false;
                }
                if(data.temp > parseFloat($('set-temp').value) && !prevTempWarning) {
                    addLog("Cáº¢NH BÃO: Nhiá»‡t Ä‘á»™ quÃ¡ cao (" + data.temp + "Â°C)!", "warning");
                    prevTempWarning = true;
                } else if (data.temp <= parseFloat($('set-temp').value)) {
                    prevTempWarning = false;
                }
            }
            prevPump = data.pump;
            isFirstUpdate = false;
            // --- End Event Logging ---

            // Mode labels update
            $('mode-label').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('led-auto-badge').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('led-auto-badge').style.background = isAutoMode
                ? 'var(--primary-container)' : 'var(--tertiary-fixed)';
            $('led-auto-badge').style.color = isAutoMode
                ? 'var(--on-primary-container)' : 'var(--tertiary)';

            $('fan-auto-badge').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('fan-auto-badge').style.background = isAutoMode
                ? 'var(--primary-container)' : 'var(--tertiary-fixed)';
            $('fan-auto-badge').style.color = isAutoMode
                ? 'var(--on-primary-container)' : 'var(--tertiary)';

            if (!isAutoMode) {
                swPump.style.pointerEvents = 'auto';
                swLed.style.pointerEvents = 'auto';
                swFan.style.pointerEvents = 'auto';
            } else {
                swPump.style.pointerEvents = 'none';
                swLed.style.pointerEvents = 'none';
                swFan.style.pointerEvents = 'none';
            }

            // --- Soil Hero ---
            $('v-soil').textContent = soil;
            const soilBadge = $('soil-badge');
            if (isPumpOn) {
                soilBadge.textContent = 'Äang tÆ°á»›i';
                soilBadge.className = 'status-badge active';
            } else if (soil < 40 && water <= 10) {
                soilBadge.textContent = 'Lá»—i';
                soilBadge.className = 'status-badge error';
            } else {
                soilBadge.textContent = 'Cháº¿ Ä‘á»™ chá»';
                soilBadge.className = 'status-badge standby';
            }

            // --- Climate ---
            $('v-temp').textContent = temp.toFixed(1) + 'Â°C';
            $('v-hum').textContent = hum.toFixed(1) + '%';

            const chipEl = $('climate-chip');
            if (isPumpOn && temp > 35) {
                chipEl.innerHTML = `<div class="alert-chip warning">
                    <span class="material-symbols-outlined">local_fire_department</span>
                    TÄƒng 20% thá»i gian tÆ°á»›i
                </div>`;
            } else if (isPumpOn && hum > 80) {
                chipEl.innerHTML = `<div class="alert-chip caution">
                    <span class="material-symbols-outlined">water</span>
                    Giáº£m tÆ°á»›i (Ä‘á»™ áº©m cao)
                </div>`;
            } else if (temp > 35) {
                chipEl.innerHTML = `<div class="alert-chip warning">
                    <span class="material-symbols-outlined">local_fire_department</span>
                    Cáº£nh bÃ¡o nhiá»‡t Ä‘á»™ cao
                </div>`;
            } else {
                chipEl.innerHTML = `<div class="alert-chip info">
                    <span class="material-symbols-outlined">check_circle</span>
                    Äiá»u kiá»‡n bÃ¬nh thÆ°á»ng
                </div>`;
            }

            // --- Lighting & Fan ---
            $('v-light').textContent = light.toLocaleString();

            const fanIcon = $('fan-icon');
            if (isFanOn) {
                fanIcon.style.animation = 'spin 1s linear infinite';
                fanIcon.style.color = 'var(--primary)';
            } else {
                fanIcon.style.animation = 'none';
                fanIcon.style.color = 'inherit';
            }

            if (!$('spin-style')) {
                const style = document.createElement('style');
                style.id = 'spin-style';
                style.innerHTML = `@keyframes spin { 100% { transform: rotate(360deg); } }`;
                document.head.appendChild(style);
            }

            $('mode-status').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            $('pump-status').textContent = isPumpOn ? 'Äang cháº¡y' : 'Táº¯t';
            $('led-status').textContent = isLedOn ? 'Äang báº­t' : 'Táº¯t';
            if ($('fan-status')) $('fan-status').textContent = isFanOn ? 'Äang cháº¡y' : 'Táº¯t';

            // --- Water Tank ---
            const tankCard = $('card-tank');
            const tankFill = $('bar-water');
            const tankDisplay = $('v-water-display');
            const tankMsg = $('tank-msg');
            const tankIcon = $('tank-icon');

            tankDisplay.textContent = water + '%';
            tankFill.style.width = water + '%';

            if (water <= 10) {
                tankCard.className = 'card tank-card';
                tankFill.className = 'fill error';
                tankDisplay.className = 'tank-level error';
                tankIcon.textContent = 'water_lock';
                tankMsg.innerHTML = `
                    <span class="material-symbols-outlined">warning</span>
                    <span>Má»±c nÆ°á»›c tháº¥p. ÄÃ£ táº¯t bÆ¡m Ä‘á»ƒ báº£o vá»‡ thiáº¿t bá»‹.</span>`;
                tankMsg.style.color = 'var(--error)';
            } else if (water <= 25) {
                tankCard.className = 'card tank-card';
                tankFill.className = 'fill warning';
                tankDisplay.className = 'tank-level warning';
                tankIcon.textContent = 'water_lock';
                tankMsg.innerHTML = `
                    <span class="material-symbols-outlined">info</span>
                    <span>Má»±c nÆ°á»›c Ä‘ang xuá»‘ng tháº¥p. HÃ£y chÃ¢m thÃªm nÆ°á»›c.</span>`;
                tankMsg.style.color = 'var(--tertiary)';
            } else {
                tankCard.className = 'card tank-card normal';
                tankFill.className = 'fill';
                tankDisplay.className = 'tank-level ok';
                tankIcon.textContent = 'water_full';
                tankMsg.innerHTML = `
                    <span class="material-symbols-outlined">check_circle</span>
                    <span>Má»±c nÆ°á»›c Ä‘áº§y Ä‘á»§. Há»‡ thá»‘ng hoáº¡t Ä‘á»™ng bÃ¬nh thÆ°á»ng.</span>`;
                tankMsg.style.color = 'var(--on-surface-variant)';
            }

            // Update additional views
            if($('v-temp-sensor')) $('v-temp-sensor').textContent = temp.toFixed(1) + 'Â°C';
            if($('v-hum-sensor')) $('v-hum-sensor').textContent = hum.toFixed(1) + '%';
            if($('v-soil-sensor')) $('v-soil-sensor').textContent = soil + '%';
            if($('v-light-sensor')) $('v-light-sensor').textContent = light.toLocaleString() + ' Lux';
            if($('v-tank-sensor')) $('v-tank-sensor').textContent = water + '%';
            if($('mode-status')) $('mode-status').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';
            if($('pump-status')) $('pump-status').textContent = isPumpOn ? 'Äang cháº¡y' : 'ÄÃ£ táº¯t';
            if($('led-status')) $('led-status').textContent = isLedOn ? 'Äang sÃ¡ng' : 'ÄÃ£ táº¯t';
            if($('schedule-mode')) $('schedule-mode').textContent = isAutoMode ? 'Tá»± Ä‘á»™ng' : 'Thá»§ cÃ´ng';

            // Notification dot
            const hasAlert = (soil < 40 && water <= 10) || temp > 35 || water <= 10;
            $('notif-dot').style.display = hasAlert ? 'block' : 'none';
        }

        // ========== CHART.JS SIMULATION ==========
        function initChart() {
            const ctx = document.getElementById('historyChart');
            if(!ctx) return;
            
            // Generate 100 data points
            const labels = [];
            const tempData = [];
            const soilData = [];
            let currentTemp = 25.0;
            let currentSoil = 60.0;
            
            for(let i = 100; i >= 1; i--) {
                labels.push(i + 'm trÆ°á»›c');
            }
            
            for(let i=0; i<100; i++) {
                // Random walk
                currentTemp += (Math.random() - 0.5) * 1.5;
                if(currentTemp < 15) currentTemp = 15;
                if(currentTemp > 40) currentTemp = 40;
                
                currentSoil += (Math.random() - 0.5) * 2 - 0.1;
                if(currentSoil < 30) currentSoil += 25; // simulate watering
                if(currentSoil > 90) currentSoil = 90;
                
                tempData.push(currentTemp.toFixed(1));
                soilData.push(currentSoil.toFixed(1));
            }
            
            new Chart(ctx, {
                type: 'line',
                data: {
                    labels: labels,
                    datasets: [
                        {
                            label: 'Nhiá»‡t Ä‘á»™ (Â°C)',
                            data: tempData,
                            borderColor: '#ba1a1a',
                            backgroundColor: 'rgba(186, 26, 26, 0.1)',
                            borderWidth: 2,
                            fill: true,
                            tension: 0.4
                        },
                        {
                            label: 'Äá»™ áº©m Ä‘áº¥t (%)',
                            data: soilData,
                            borderColor: '#006399',
                            backgroundColor: 'rgba(0, 99, 153, 0.1)',
                            borderWidth: 2,
                            fill: true,
                            tension: 0.4
                        }
                    ]
                },
                options: {
                    responsive: true,
                    interaction: {
                        mode: 'index',
                        intersect: false,
                    },
                    scales: {
                        y: {
                            beginAtZero: false
                        }
                    }
                }
            });
        }
        
        window.addEventListener('DOMContentLoaded', initChart);
        
    </script>
</body>
</html>

)rawliteral";

#endif
